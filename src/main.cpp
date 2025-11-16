// #include "parser.h"
// #include <iostream>
// #include <string>
// #include <sstream>

// int main()
// {
//     std::string input;
//     std::string line;

//     while (std::getline(std::cin, line))
//     {
//         input += line + "\n";
//     }

//     Parser parser(input);
//     parser.parse();
//     parser.printErrors();

//     return 0;
// }

#include "parser.h"
#include "lexer.h"
#include <iostream>
#include <string>
#include <sstream>

int main()
{
    std::stringstream buffer;
    buffer << std::cin.rdbuf();
    std::string sourceCode = buffer.str();

    Lexer lexer(sourceCode);
    std::vector<Token> tokens = lexer.getAllTokens();

    bool hasLexicalError = false;
    std::set<int> errorLines;
    for (const auto &token : tokens)
    {
        if (token.type == TokenType::UNKNOWN)
        {
            hasLexicalError = true;
            errorLines.insert(token.line);
        }
    }

    if (hasLexicalError)
    {
        std::cout << "reject" << std::endl;
        for (int line : errorLines)
        {
            std::cout << line << std::endl;
        }
        return 0;
    }

    Parser parser(std::move(tokens));
    parser.parse();
    parser.printErrors();

    return 0;
}