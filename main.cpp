#include "VelaVM.h"

inline std::vector<unsigned char> read_file_bytes(const std::string& path)
{
    // 以二进制打开，定位到末尾
    std::ifstream ifs(path, std::ios::binary | std::ios::ate);
    if (!ifs)
        throw std::runtime_error("cannot open file: " + path);

    std::streamsize size = ifs.tellg();
    if (size < 0)
        throw std::runtime_error("cannot get file size: " + path);

    ifs.seekg(0, std::ios::beg);

    std::vector<unsigned char> buffer(static_cast<size_t>(size));

    if (size > 0)
    {
        if (!ifs.read(reinterpret_cast<char*>(buffer.data()), size))
            throw std::runtime_error("cannot read file: " + path);
    }

    return buffer;
}

int main()
{
    try
    {
        std::vector<unsigned char> code = vela::read_file_bytes("program.vela");

        vela::VelaVM vm(std::move(code));
        int ret = vm.run();
        std::cout << "\nreturn: " << ret << '\n';
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return 1;
    }
    return 0;
}
