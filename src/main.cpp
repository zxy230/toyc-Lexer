#include <iostream>
#include <string>
#include <sstream>
#include "lexer.h"

std::string readInput()
{
    std::ostringstream buffer;
    buffer << std::cin.rdbuf();
    return buffer.str();
}

int main()
{
    try
    {
        std::string input = readInput();

        if (input.empty())
        {
            std::cerr << "Error: Empty input" << std::endl;
            return 1;
        }

        Lexer lexer(input);
        auto tokens = lexer.tokenize();

        // 输出token流
        for (size_t i = 0; i < tokens.size(); i++)
        {
            const auto &token = tokens[i];
            std::cout << i << ":" << tokenTypeToString(token.type) << ":\"" << token.value << "\"" << std::endl;
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}