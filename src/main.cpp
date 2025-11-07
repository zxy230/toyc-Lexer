#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <unordered_set>
#include <vector>
#include "lexer.h"
#include "parser.h"

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
            std::cout << "reject" << std::endl;
            std::cout << "1" << std::endl;
            return 0;
        }

        // 词法分析
        Lexer lexer(input);
        auto tokens = lexer.tokenize();

        // 移除UNKNOWN token（如果有）
        tokens.erase(
            std::remove_if(tokens.begin(), tokens.end(),
                           [](const Token &token)
                           { return token.type == TokenType::UNKNOWN; }),
            tokens.end());

        if (tokens.empty())
        {
            std::cout << "reject" << std::endl;
            std::cout << "1" << std::endl;
            return 0;
        }

        // 语法分析
        Parser parser(tokens);
        bool success = parser.parse();

        if (success)
        {
            std::cout << "accept" << std::endl;
        }
        else
        {
            std::cout << "reject" << std::endl;
            const auto &errors = parser.getErrors();

            // 按行号升序排序并去重
            std::vector<int> sortedErrors = errors;
            std::sort(sortedErrors.begin(), sortedErrors.end());
            auto last = std::unique(sortedErrors.begin(), sortedErrors.end());
            sortedErrors.erase(last, sortedErrors.end());

            for (int line : sortedErrors)
            {
                std::cout << line << std::endl;
            }
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}