#ifndef VELAVM_H
#define VELAVM_H

#include <fstream>
#include <vector>
#include <cstdint>
#include <iterator>
#include <stdint.h>
#include <cstdlib>
#include <string>
#include <stdexcept>
#include <iostream>
#include <cstring>

#include "VMByteCode.h"



namespace vela
{

union BasicValue
{
    // 布尔
    bool                 b;

    // 字符
    char                 c;
    signed char          sc;
    unsigned char        uc;
    wchar_t              wc;
    char16_t             c16;
    char32_t             c32;

    // 整型（有符号）
    short                s;
    int                  i;
    long                 l;
    long long            ll;

    // 整型（无符号）
    unsigned short       us;
    unsigned int         ui;
    unsigned long        ul;
    unsigned long long   ull;

    // 浮点
    float                f;
    double               d;
    long double          ld;

    // 指针 / 空值（也算标量类型）
    void*                ptr;
    std::nullptr_t       np;

};

inline void vmErr(const std::string& message)
{
    throw std::runtime_error("VelaVM Error: " + message);
}

class VelaVM
{
    std::vector<unsigned char> code_storage;
    unsigned char* code_raw;
    size_t code_raw_size;
    std::vector<BasicValue> op_stream;
    char* stack = nullptr;
    size_t stack_point;
    size_t stack_size = 16384;
    int ret_value = 0;

    bool stop = false;

public:
    explicit VelaVM(const std::vector<unsigned char>& raw)
        : code_storage(raw),
          code_raw(code_storage.data()),
          code_raw_size(code_storage.size())
    {
        stack_point = stack_size;
        stack = (char*)malloc(stack_size);

        if (!stack) vmErr("out of memory");
    }

    ~VelaVM()
    {
        free(stack);
    }

    VelaVM(const VelaVM&) = delete;
    VelaVM& operator=(const VelaVM&) = delete;

    int run()
    {
        if (code_raw_size == 0) return -1;

        size_t index = 0;

        while(!stop && index < code_raw_size)
        {
            run_command(index);
            index++;
        }

        return ret_value;
    }

private:
    void stack_push(const void* v, size_t data_len)
    {
        if (data_len > stack_point) vmErr("stack overflow");

        stack_point -= data_len;
        std::memcpy(stack + stack_point, v, data_len);
    }

    void stack_pop(void* out, size_t data_len)
    {
        if (data_len > stack_size - stack_point) vmErr("stack underflow");

        std::memcpy(out, stack + stack_point, data_len);
        stack_point += data_len;
    }

    void run_command(size_t& index)
    {
        unsigned char code = code_raw[index];

        switch(code)
        {
        case (int)vmByteCode::ipushop:
        {
            constexpr size_t N = sizeof(int);   // 4

            if (index + N >= code_raw_size)
                vmErr("ipushop: operand out of range");

            // 小端拼装
            uint32_t raw = 0;

            for (size_t k = 0; k < N; ++k)
                raw |= (uint32_t)code_raw[index + 1 + k] << (8 * k);

            index += N;

            BasicValue v;
            v.i = (int)raw;
            op_stream.push_back(v);
            break;
        }

        case (int)vmByteCode::ipopop:
        {
            if (op_stream.empty()) vmErr("ipopop: empty");

            op_stream.pop_back();
            break;
        }

        case (int)vmByteCode::iadd:
        {
            if (op_stream.size() < 2)
                vmErr("operator count less than 2 with iadd");

            int a = op_stream[op_stream.size() - 2].i;
            int b = op_stream[op_stream.size() - 1].i;
            op_stream.pop_back();
            op_stream.pop_back();
            int result = a + b;
            stack_push(&result, sizeof(int));
            break;
        }

        case (int)vmByteCode::iprint:
        {
            if (op_stream.empty())
                vmErr("operator count less than 1 with iprint");

            std::cout << op_stream.back().i;
            break;
        }

        case (int)vmByteCode::ist2op:
        {
            int value;
            stack_pop(&value, sizeof(int));
            BasicValue v;
            v.i =  value;
            op_stream.push_back(v);
            break;
        }

        case (int)vmByteCode::iop2st:
        {
            if (op_stream.empty())
            {
                vmErr("try to use iop2st when oprator stream empty.");
            }

            BasicValue v;
            v.i = op_stream.back().i;
            op_stream.pop_back();
            stack_push(&v.i, sizeof(int));
            break;
        }

        case (int)vmByteCode::stop:
        {
            stop = true;
            break;
        }

        case (int)vmByteCode::iret: {
            ret_value = op_stream.back().i; op_stream.pop_back();
            stop = true;
            break;
        }

        default:
            vmErr("unknown opcode");
        }

        return;
    }
};

}


#endif // VELAVM_H
