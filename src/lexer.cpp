#include "lexer.h"
#include <cctype>

std::string tokenTypeToString(TokenType type)
{
    switch (type)
    {
    // 关键字
    case TokenType::KEYWORD_INT:
        return "'int'";
    case TokenType::KEYWORD_VOID:
        return "'void'";
    case TokenType::KEYWORD_IF:
        return "'if'";
    case TokenType::KEYWORD_ELSE:
        return "'else'";
    case TokenType::KEYWORD_WHILE:
        return "'while'";
    case TokenType::KEYWORD_BREAK:
        return "'break'";
    case TokenType::KEYWORD_CONTINUE:
        return "'continue'";
    case TokenType::KEYWORD_RETURN:
        return "'return'";

    // 标识符和常量
    case TokenType::IDENTIFIER:
        return "Ident";
    case TokenType::INT_CONST:
        return "IntConst";

    // 运算符和分隔符
    case TokenType::PLUS:
        return "'+'";
    case TokenType::MINUS:
        return "'-'";
    case TokenType::MULTIPLY:
        return "'*'";
    case TokenType::DIVIDE:
        return "'/'";
    case TokenType::MODULO:
        return "'%'";
    case TokenType::ASSIGN:
        return "'='";
    case TokenType::EQUALS:
        return "'=='";
    case TokenType::NOT_EQUALS:
        return "'!='";
    case TokenType::LESS:
        return "'<'";
    case TokenType::GREATER:
        return "'>'";
    case TokenType::LESS_EQUAL:
        return "'<='";
    case TokenType::GREATER_EQUAL:
        return "'>='";
    case TokenType::AND:
        return "'&&'";
    case TokenType::OR:
        return "'||'";
    case TokenType::NOT:
        return "'!'";

    // 分隔符
    case TokenType::LEFT_PAREN:
        return "'('";
    case TokenType::RIGHT_PAREN:
        return "')'";
    case TokenType::LEFT_BRACE:
        return "'{'";
    case TokenType::RIGHT_BRACE:
        return "'}'";
    case TokenType::SEMICOLON:
        return "';'";
    case TokenType::COMMA:
        return "','";

    default:
        return "Unknown";
    }
}

Lexer::Lexer(const std::string &input)
    : input(input), pos(0), line(1), col(1)
{
}

std::vector<Token> Lexer::tokenize()
{
    std::vector<Token> tokens;

    while (!isAtEnd())
    {
        skipWhitespace();
        if (isAtEnd())
            break;

        Token token = readNextToken();
        if (token.type != TokenType::UNKNOWN)
        {
            tokens.push_back(token);
        }
    }

    return tokens;
}

Token Lexer::readNextToken()
{
    char c = current();

    if (isIdentifierStart(c))
    {
        return readIdentifier();
    }
    else if (isDigit(c) || (c == '-' && isDigit(peek())))
    {
        return readNumber();
    }
    else
    {
        return readOperator();
    }
}

// 获取当前字符
char Lexer::current() const
{
    return pos < input.length() ? input[pos] : '\0';
}

// 向前查看字符
char Lexer::peek(int offset) const
{
    size_t peekPos = pos + offset;
    return peekPos < input.length() ? input[peekPos] : '\0';
}

// 前进一个字符
void Lexer::advance()
{
    if (!isAtEnd())
    {
        if (current() == '\n')
        {
            line++;
            col = 1;
        }
        else
        {
            col++;
        }
        pos++;
    }
}

// 检查是否到达输入末尾
bool Lexer::isAtEnd() const
{
    return pos >= input.length();
}

// 匹配并消费一个字符（如果匹配）
bool Lexer::match(char expected)
{
    if (isAtEnd() || current() != expected)
    {
        return false;
    }
    advance();
    return true;
}

// 标识符起始字符
bool Lexer::isIdentifierStart(char c)
{
    return (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z') ||
           c == '_';
}

// 标识符后续字符
bool Lexer::isIdentifierPart(char c)
{
    return isIdentifierStart(c) || (c >= '0' && c <= '9');
}

// 数字字符
bool Lexer::isDigit(char c)
{
    return c >= '0' && c <= '9';
}

void Lexer::skipWhitespace()
{
    while (!isAtEnd())
    {
        char c = current();
        if (c == ' ' || c == '\t' || c == '\r' || c == '\n')
        {
            advance();
        }
        else if (c == '/')
        {
            // 可能是注释开始
            if (peek() == '/' || peek() == '*')
            {
                skipComment();
            }
            else
            {
                break;
            }
        }
        else
        {
            break;
        }
    }
}

void Lexer::skipComment()
{
    if (match('/'))
    {
        // 单行注释：跳过直到行尾
        while (!isAtEnd() && current() != '\n')
        {
            advance();
        }
    }
    else if (match('*'))
    {
        // 多行注释：跳过直到 */
        while (!isAtEnd())
        {
            if (current() == '*' && peek() == '/')
            {
                advance(); // 跳过 *
                advance(); // 跳过 /
                break;
            }
            advance();
        }
    }
}