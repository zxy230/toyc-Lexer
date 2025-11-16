#include "parser.h"
#include <iostream>
#include <algorithm>

Parser::Parser(std::vector<Token> tokens)
    : m_tokens(std::move(tokens)),
      m_index(0),
      hasMain(false)
{
}

Token Parser::current() const
{
    return m_tokens[m_index];
}

Token Parser::previous() const
{
    return m_tokens[m_index - 1];
}

bool Parser::isAtEnd() const
{
    return m_tokens[m_index].type == TokenType::END_OF_FILE;
}

void Parser::advance()
{
    if (!isAtEnd())
    {
        m_index++;
    }
}

bool Parser::check(TokenType type) const
{
    if (isAtEnd())
        return false;
    return current().type == type;
}

bool Parser::match(TokenType type)
{
    if (check(type))
    {
        advance();
        return true;
    }
    return false;
}

void Parser::reportError(const std::string &msg)
{
    const int errorLine = current().line;

    bool exists = std::any_of(m_errors.begin(), m_errors.end(),
                              [errorLine](const ErrorInfo &e)
                              { return e.line == errorLine; });

    if (!exists)
    {
        m_errors.emplace_back(errorLine, msg);
    }

    throw ParseError{};
}

void Parser::requireToken(TokenType type, const std::string &expected)
{
    if (match(type))
    {
        return;
    }
    reportError("Expected " + expected + ", but got '" + current().value + "'");
}

void Parser::recoverToStatement()
{
    while (!isAtEnd())
    {
        if (previous().type == TokenType::SEMICOLON)
            return;

        switch (current().type)
        {
        case TokenType::IF:
        case TokenType::WHILE:
        case TokenType::RETURN:
        case TokenType::BREAK:
        case TokenType::CONTINUE:
        case TokenType::INT:
        case TokenType::VOID:
        case TokenType::LEFT_BRACE:
        case TokenType::RIGHT_BRACE:
            return;
        default:;
        }
        advance();
    }
}

void Parser::recoverToFunction()
{
    while (!isAtEnd())
    {
        if (check(TokenType::INT) || check(TokenType::VOID))
        {
            return;
        }

        if (check(TokenType::LEFT_BRACE))
        {
            advance();
            int braceDepth = 1;
            while (!isAtEnd() && braceDepth > 0)
            {
                if (match(TokenType::LEFT_BRACE))
                    braceDepth++;
                else if (match(TokenType::RIGHT_BRACE))
                    braceDepth--;
                else
                    advance();
            }
            continue;
        }

        advance();
    }
}
bool Parser::parse()
{
    try
    {
        analyzeProgram();
    }
    catch (const ParseError &)
    {
    }
    return m_errors.empty();
}

void Parser::analyzeProgram()
{
    if (isAtEnd())
    {
        reportError("Empty program");
    }

    while (!isAtEnd())
    {
        try
        {
            analyzeFunction();
        }
        catch (const ParseError &)
        {
            recoverToFunction();
        }
    }

    if (!hasMain && m_errors.empty())
    {
        m_errors.emplace_back(1, "Missing main function");
    }
}

void Parser::analyzeFunction()
{
    if (!check(TokenType::INT) && !check(TokenType::VOID))
    {
        reportError("Expected 'int' or 'void' at function start");
    }
    advance();

    requireToken(TokenType::IDENTIFIER, "function name");
    const std::string funcName = previous().value;

    if (funcName == "main")
    {
        hasMain = true;
    }

    if (functionNames.count(funcName))
    {
        reportError("Duplicate function name");
    }
    else
    {
        functionNames.insert(funcName);
    }

    requireToken(TokenType::LEFT_PAREN, "(");
    analyzeParameters();
    requireToken(TokenType::RIGHT_PAREN, ")");
    analyzeCodeBlock();
}

void Parser::analyzeParameters()
{
    if (check(TokenType::RIGHT_PAREN))
    {
        return;
    }

    if (check(TokenType::COMMA))
    {
        reportError("Expected parameter, not comma");
    }

    do
    {
        analyzeSingleParam();
    } while (match(TokenType::COMMA));
}

void Parser::analyzeSingleParam()
{
    requireToken(TokenType::INT, "parameter type 'int'");
    requireToken(TokenType::IDENTIFIER, "parameter name");
}

void Parser::analyzeCodeBlock()
{
    requireToken(TokenType::LEFT_BRACE, "{");

    while (!check(TokenType::RIGHT_BRACE) && !isAtEnd())
    {
        try
        {
            analyzeStatement();
        }
        catch (const ParseError &)
        {
            recoverToStatement();
        }
    }

    requireToken(TokenType::RIGHT_BRACE, "}");
}

// *** 这是修复后的函数 ***
void Parser::analyzeStatement()
{
    if (check(TokenType::LEFT_BRACE))
    {
        analyzeCodeBlock();
    }
    else if (match(TokenType::SEMICOLON))
    {
        // 空语句
    }
    else if (check(TokenType::INT))
    {
        analyzeVariableDeclaration();
    }
    else if (check(TokenType::IDENTIFIER))
    {
        analyzeAssignmentOrCall();
    }
    else if (check(TokenType::IF))
    {
        analyzeConditional();
    }
    else if (check(TokenType::WHILE))
    {
        analyzeLoop();
    }
    else if (check(TokenType::BREAK) || check(TokenType::CONTINUE))
    {
        analyzeJumpStatement();
    }
    else if (check(TokenType::RETURN))
    {
        advance(); // 消耗 'return'
        analyzeExpression();
        requireToken(TokenType::SEMICOLON, ";");
    }
    else if (check(TokenType::ELSE))
    {
        reportError("Stray 'else' without 'if'");
        advance(); // 消耗 else 以便恢复
    }
    else
    {
        analyzeExpression();
        requireToken(TokenType::SEMICOLON, ";");
    }
}

void Parser::analyzeVariableDeclaration()
{
    advance(); // 消耗 'int'
    do
    {
        requireToken(TokenType::IDENTIFIER, "variable name");
        if (match(TokenType::ASSIGN))
        {
            if (check(TokenType::SEMICOLON) || check(TokenType::COMMA))
            {
                reportError("Missing expression after '='");
            }
            analyzeExpression();
        }
        if (!check(TokenType::COMMA))
            break;
        advance();
        if (check(TokenType::SEMICOLON))
        {
            reportError("Missing variable name after ','");
        }
    } while (true);
    requireToken(TokenType::SEMICOLON, ";");
}

void Parser::analyzeAssignmentOrCall()
{
    advance(); // 消耗 'identifier'
    Token idToken = previous();

    if (match(TokenType::ASSIGN))
    {
        analyzeExpression();
        requireToken(TokenType::SEMICOLON, ";");
    }
    else if (match(TokenType::LEFT_PAREN))
    {
        analyzeFunctionArguments();
        requireToken(TokenType::RIGHT_PAREN, ")");
        requireToken(TokenType::SEMICOLON, ";");
    }
    else
    {
        reportError("Expected '=' or '(' after identifier '" + idToken.value + "'");
    }
}

void Parser::analyzeConditional()
{
    advance(); // 消耗 'if'
    requireToken(TokenType::LEFT_PAREN, "(");
    analyzeExpression();
    requireToken(TokenType::RIGHT_PAREN, ")");
    analyzeStatement();
    if (match(TokenType::ELSE))
    {
        analyzeStatement();
    }
}

void Parser::analyzeLoop()
{
    advance(); // 消耗 'while'
    requireToken(TokenType::LEFT_PAREN, "(");
    analyzeExpression();
    requireToken(TokenType::RIGHT_PAREN, ")");
    analyzeStatement();
}

void Parser::analyzeJumpStatement()
{
    advance(); // 消耗 'break' 或 'continue'
    requireToken(TokenType::SEMICOLON, ";");
}

void Parser::analyzeFunctionArguments()
{
    if (check(TokenType::RIGHT_PAREN))
        return;
    if (check(TokenType::COMMA))
        reportError("Missing argument");
    do
    {
        analyzeExpression();
    } while (match(TokenType::COMMA));
}

void Parser::analyzeExpression()
{
    analyzeLogicalOr();
}

void Parser::analyzeLogicalOr()
{
    analyzeLogicalAnd();
    while (match(TokenType::OR))
        analyzeLogicalAnd();
}

void Parser::analyzeLogicalAnd()
{
    analyzeComparison();
    while (match(TokenType::AND))
        analyzeComparison();
}

void Parser::analyzeComparison()
{
    analyzeAddition();
    while (match(TokenType::LESS) || match(TokenType::GREATER) ||
           match(TokenType::LESS_EQUAL) || match(TokenType::GREATER_EQUAL) ||
           match(TokenType::EQUAL) || match(TokenType::NOT_EQUAL))
    {
        analyzeAddition();
    }
}

void Parser::analyzeAddition()
{
    analyzeMultiplication();
    while (match(TokenType::PLUS) || match(TokenType::MINUS))
    {
        if (check(TokenType::SEMICOLON) || check(TokenType::RIGHT_PAREN))
        {
            reportError("Missing operand");
        }
        analyzeMultiplication();
    }
}

void Parser::analyzeMultiplication()
{
    analyzeUnary();
    while (match(TokenType::MULTIPLY) || match(TokenType::DIVIDE) || match(TokenType::MODULO))
    {
        if (check(TokenType::SEMICOLON) || check(TokenType::RIGHT_PAREN))
        {
            reportError("Missing operand");
        }
        analyzeUnary();
    }
}

void Parser::analyzeUnary()
{
    if (match(TokenType::PLUS) || match(TokenType::MINUS) || match(TokenType::NOT))
    {
        if (check(TokenType::SEMICOLON) || check(TokenType::RIGHT_PAREN))
        {
            reportError("Missing operand");
        }
        analyzeUnary();
    }
    else
    {
        analyzePrimary();
    }
}

void Parser::analyzePrimary()
{
    if (match(TokenType::IDENTIFIER))
    {
        if (match(TokenType::LEFT_PAREN))
        {
            analyzeFunctionArguments();
            requireToken(TokenType::RIGHT_PAREN, ")");
        }
        return;
    }
    if (match(TokenType::INTCONST))
    {
        return;
    }
    if (match(TokenType::LEFT_PAREN))
    {
        analyzeExpression();
        requireToken(TokenType::RIGHT_PAREN, ")");
        return;
    }
    reportError("Expected expression (identifier, number, or '()')");
}

void Parser::printErrors()
{
    if (m_errors.empty())
    {
        std::cout << "accept" << std::endl;
        return;
    }

    std::cout << "reject" << std::endl;

    std::set<int> printedLines;
    for (const auto &e : m_errors)
    {
        if (printedLines.insert(e.line).second)
        {
            std::cout << e.line << std::endl;
        }
    }
}