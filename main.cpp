#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <conio.h> // 必须包含此头文件

enum command
{
    PADD,    // >  Memory Point ++
    PSUB,    // <  Memory Point --
    VADD,    // +  Value ++
    VSUB,    // -  Value --
    VIN,     // ,  Get input
    VOUT,    // .  Print value
    LMSTART, // [  Loop Start
    LMEND,   // ]  Loop End
    PGEND,   // (程序结束标记)
};

// 解析 BF 源代码
std::vector<command> parser(const std::string& strcmd)
{
    std::vector<command> out;
    out.reserve(strcmd.size());  // 预分配空间，提高性能

    for (char c : strcmd)  // 使用范围 for 循环更简洁
    {
        switch (c)
        {
        case '>':
            out.push_back(PADD);
            break;
        case '<':
            out.push_back(PSUB);
            break;
        case '+':
            out.push_back(VADD);
            break;
        case '-':
            out.push_back(VSUB);
            break;
        case ',':
            out.push_back(VIN);
            break;
        case '.':
            out.push_back(VOUT);
            break;
        case '[':
            out.push_back(LMSTART);
            break;
        case ']':
            out.push_back(LMEND);
            break;
        // 忽略其他字符（空格、换行、注释等）
        default:
            break;
        }
    }

    return out;
}

// 执行 BF 程序
void run(const std::vector<command>& program)
{
    unsigned char memory[4096] = {0};
    unsigned short memory_point = 0;
    unsigned short commands_point = 0;
    std::vector<unsigned short> loop_stack;

    while (commands_point < program.size())
    {
        command comm = program[commands_point];

        switch (comm)
        {
        case PADD:
            memory_point++;
            if (memory_point >= 4096)
            {
                std::cerr << "Error! Memory point overflow." << std::endl;
                return;
            }
            break;

        case PSUB:
            if (memory_point == 0)
            {
                std::cerr << "Error! Memory point underflow." << std::endl;
                return;
            }
            memory_point--;
            break;

        case VADD:
            memory[memory_point]++;
            break;

        case VSUB:
            memory[memory_point]--;
            break;

        case VOUT:
            std::cout << memory[memory_point];
            break;

        case VIN:
            memory[memory_point] = _getch();
            break;

        case LMSTART:
            if (memory[memory_point] == 0)
            {
                // 跳过循环体
                int depth = 1;
                while (depth > 0)
                {
                    commands_point++;
                    if (commands_point >= program.size())
                    {
                        std::cerr << "Error! Unmatched loop start." << std::endl;
                        return;
                    }
                    if (program[commands_point] == LMSTART) depth++;
                    if (program[commands_point] == LMEND) depth--;
                }
            }
            else
            {
                loop_stack.push_back(commands_point);
            }
            break;

        case LMEND:
            if (memory[memory_point] != 0)
            {
                if (loop_stack.empty())
                {
                    std::cerr << "Error! Unmatched loop end." << std::endl;
                    return;
                }
                commands_point = loop_stack.back();
            }
            else
            {
                if (!loop_stack.empty())
                {
                    loop_stack.pop_back();
                }
            }
            break;

        case PGEND:
            return;

        default:
            std::cerr << "Error! Unknown command." << std::endl;
            return;
        }

        commands_point++;
    }
}

int main()
{
    // Hello World BF 代码
    std::string bf_code =
        "++++++++++[>+++++++>++++++++++>+++>+<<<<-]>++.>+.+++++++..+++.>++.<<+++++++++++++++.>.+++.------.--------.>+.>.>>>.";

    // 解析并执行
    std::vector<command> program = parser(bf_code);

    std::cout << "Parsed " << program.size() << " instructions." << std::endl;
    std::cout << "Output: ";
    run(program);
    std::cout << std::endl;

    return 0;
}
