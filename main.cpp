#include <iostream>

struct context
{
    std::string str = "";
};

void func1(context* c)
{
    std::cout << c->str << 1 << std::endl;
}

void func2(context* c)
{
    std::cout << c->str << 2 << std::endl;
}

void func3(context* c)
{
    std::cout << c->str << 3 << std::endl;
}

typedef void (*FuncPtr)(context*);
int main()
{
    FuncPtr functions[] = {func1, func2, func3};

    context c;
    c.str = "Hello World!";


    for(int i = 0; i < 3 ; i++)
    {
        functions[i](&c);
    }

    return 0;
}
