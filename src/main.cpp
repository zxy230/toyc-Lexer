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

        size_t index = 0;
        // 输出token流
        for (const auto &token : tokens)
        {
            std::cout << index << ":" << tokenTypeToString(token.type) << ":\"" << token.value << "\"" << std::endl;
            index++;
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}