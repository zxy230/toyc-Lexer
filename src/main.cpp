// src/main.cpp
#include "Lexer.h"
#include <iostream>
#include <vector>

int main()
{
    Lexer lexer(std::cin);
    auto tokens = lexer.tokenize();

    for (size_t i = 0; i < tokens.size(); ++i)
    {
        std::cout << i << ":" << tokenTypeToString(tokens[i].type)
                  << ":\"" << tokens[i].lexeme << "\"\n";
    }

    return 0;
}