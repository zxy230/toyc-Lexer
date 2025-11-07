#include "lexer.h"
#include <cctype>
#include <iostream>

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
    else if (isDigit(c))
    {
        return readNumber();
    }
    else
    {
        return readOperator();
    }
}

Token Lexer::readIdentifier()
{
    int startLine = line;
    int startCol = col;
    std::string value;

    // 读取完整的标识符
    while (!isAtEnd() && isIdentifierPart(current()))
    {
        value += current();
        advance();
    }

    // 检查是否是关键字
    TokenType type = TokenType::IDENTIFIER;
    if (value == "int")
        type = TokenType::KEYWORD_INT;
    else if (value == "void")
        type = TokenType::KEYWORD_VOID;
    else if (value == "if")
        type = TokenType::KEYWORD_IF;
    else if (value == "else")
        type = TokenType::KEYWORD_ELSE;
    else if (value == "while")
        type = TokenType::KEYWORD_WHILE;
    else if (value == "break")
        type = TokenType::KEYWORD_BREAK;
    else if (value == "continue")
        type = TokenType::KEYWORD_CONTINUE;
    else if (value == "return")
        type = TokenType::KEYWORD_RETURN;

    return Token{0, type, value, startLine, startCol};
}

Token Lexer::readNumber()
{
    int startLine = line;
    int startCol = col;
    std::string value;

    // 读取数字部分
    if (current() == '0')
    {
        value += current();
        advance();
        while (!isAtEnd() && isDigit(current()))
        {
            value += current();
            advance();
        }
    }
    else
    {
        // 读取非零数字
        while (!isAtEnd() && isDigit(current()))
        {
            value += current();
            advance();
        }
    }

    return Token{0, TokenType::INT_CONST, value, startLine, startCol};
}

Token Lexer::readOperator()
{
    int startLine = line;
    int startCol = col;
    char c = current();
    advance();

    TokenType type = TokenType::UNKNOWN;
    std::string value(1, c);

    // 处理多字符运算符
    switch (c)
    {
    case '=':
        if (match('='))
        {
            type = TokenType::EQUALS;
            value = "==";
        }
        else
        {
            type = TokenType::ASSIGN;
        }
        break;
    case '!':
        if (match('='))
        {
            type = TokenType::NOT_EQUALS;
            value = "!=";
        }
        else
        {
            type = TokenType::NOT;
        }
        break;
    case '<':
        if (match('='))
        {
            type = TokenType::LESS_EQUAL;
            value = "<=";
        }
        else
        {
            type = TokenType::LESS;
        }
        break;
    case '>':
        if (match('='))
        {
            type = TokenType::GREATER_EQUAL;
            value = ">=";
        }
        else
        {
            type = TokenType::GREATER;
        }
        break;
    case '&':
        if (match('&'))
        {
            type = TokenType::AND;
            value = "&&";
        }
        break;
    case '|':
        if (match('|'))
        {
            type = TokenType::OR;
            value = "||";
        }
        break;
    case '+':
        type = TokenType::PLUS;
        break;
    case '-':
        type = TokenType::MINUS;
        break;
    case '*':
        type = TokenType::MULTIPLY;
        break;
    case '/':
        type = TokenType::DIVIDE;
        break;
    case '%':
        type = TokenType::MODULO;
        break;
    case '(':
        type = TokenType::LEFT_PAREN;
        break;
    case ')':
        type = TokenType::RIGHT_PAREN;
        break;
    case '{':
        type = TokenType::LEFT_BRACE;
        break;
    case '}':
        type = TokenType::RIGHT_BRACE;
        break;
    case ';':
        type = TokenType::SEMICOLON;
        break;
    case ',':
        type = TokenType::COMMA;
        break;
    default:
        // 未知字符，保持UNKNOWN类型
        break;
    }

    return Token{0, type, value, startLine, startCol};
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
        char c = current();
        if (c == '\r')
        {
            // 处理Windows换行符\r\n
            if (peek() == '\n')
            {
                pos++; // 跳过\n
            }
            line++;
            col = 1;
        }
        else if (c == '\n')
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
            char next = peek();
            if (next == '/') // 单行注释
            {
                advance(); // 跳过 '/'
                advance(); // 跳过 '/'
                while (!isAtEnd() && current() != '\n')
                {
                    advance();
                }
            }
            else if (next == '*') // 多行注释
            {
                advance(); // 跳过 '/'
                advance(); // 跳过 '*'
                while (!isAtEnd())
                {
                    if (current() == '*' && peek() == '/')
                    {
                        advance(); // 跳过 '*'
                        advance(); // 跳过 '/'
                        break;
                    }
                    advance();
                }
            }
            else
            {
                break; // 只是一个除号 '/'
            }
        }
        else
        {
            break;
        }
    }
}