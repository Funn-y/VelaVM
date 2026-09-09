#include <iostream>
#include <vector>
#include <string>
#include <cctype>
#include <memory>

// AST

enum class NODE_TYPE
{
    NUMBER,
    ADD,
    SUB,
    MUL,
    DIV
};

enum class TOKEN_TYPE
{
    NUMBER,
    ADD,
    SUB,
    MUL,
    DIV
};

struct Node
{
    NODE_TYPE type;
    std::string payload;                          // 如果是数字，存值
    std::unique_ptr<Node> left;                   // 左子节点
    std::unique_ptr<Node> right;                  // 右子节点

    // 构造函数：数字节点
    Node(NODE_TYPE t, const std::string& val)
        : type(t), payload(val), left(nullptr), right(nullptr) {}

    // 构造函数：操作符节点
    Node(NODE_TYPE t, std::unique_ptr<Node> l, std::unique_ptr<Node> r)
        : type(t), payload(""), left(std::move(l)), right(std::move(r)) {}
};

struct Token
{
    TOKEN_TYPE tk_type;
    std::string tk_payload;
};

std::vector<Token> lexer(std::string expr)
{
    std::vector<Token> out;

    for(size_t i = 0 ; i < expr.size() ; i++)
    {
        char now_letter = expr[i];
        Token t;
        if(now_letter == '+')
        {
            t.tk_type = TOKEN_TYPE::ADD;
            out.push_back(t);
        }

        else if (now_letter == '-')
        {
            t.tk_type = TOKEN_TYPE::SUB;
            out.push_back(t);
        }

        else if (now_letter == '*')
        {
            t.tk_type = TOKEN_TYPE::MUL;
            out.push_back(t);
        }

        else if(now_letter == '/')
        {
            t.tk_type = TOKEN_TYPE::DIV;
            out.push_back(t);
        }

        else if (isdigit(now_letter))
        {
            t.tk_type = TOKEN_TYPE::NUMBER;

// 先存入当前数字（因为能进入这里，说明 expr[i] 肯定是数字）
            t.tk_payload.push_back(expr[i]);

// 然后循环检查后面的字符
            while (true)
            {
                if(!(i+1 < expr.length()))
                {
                    break;
                }
                if(!isdigit(expr[i+1]))
                {
                    break;
                }
                i++;
                t.tk_payload.push_back(expr[i]);
            }
        }

        else
        {
            std::cerr << "Unknow token: '" << now_letter << "'." << std::endl;
        }

        std::cout << "TYPE:" << (int)t.tk_type << std::endl;
        std::cout << "PAYLOAD:" << t.tk_payload << std::endl;
    }

    return out;
}



int main()
{
    std::string expression = "1+1";
    std::vector<Token> tokens;

    tokens = lexer(expression);

    Node ast = parser(tokens);


    for(int i = 0 ; i < tokens.size() ; i++)
    {
        std::cout << "TYPE:" << (int)tokens.at(i).tk_type << std::endl;
        std::cout << "PAYLOAD:" << tokens.at(i).tk_payload << std::endl;
    }

    return 0;
}
