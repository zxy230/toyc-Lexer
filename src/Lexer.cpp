// src/Lexer.cpp
#include "Lexer.h"
#include <map>
#include <cctype>
#include <unordered_map>
#include <stdexcept>

std::string tokenTypeToString(TokenType type)
{
    switch (type)
    {
    case TokenType::KW_INT:
        return "'int'";
    case TokenType::KW_VOID:
        return "'void'";
    case TokenType::KW_IF:
        return "'if'";
    case TokenType::KW_ELSE:
        return "'else'";
    case TokenType::KW_WHILE:
        return "'while'";
    case TokenType::KW_BREAK:
        return "'break'";
    case TokenType::KW_CONTINUE:
        return "'continue'";
    case TokenType::KW_RETURN:
        return "'return'";
    case TokenType::LPAREN:
        return "'('";
    case TokenType::RPAREN:
        return "')'";
    case TokenType::LBRACE:
        return "'{'";
    case TokenType::RBRACE:
        return "'}'";
    case TokenType::SEMICOLON:
        return "';'";
    case TokenType::COMMA:
        return "','";
    case TokenType::PLUS:
        return "'+'";
    case TokenType::MINUS:
        return "'-'";
    case TokenType::STAR:
        return "'*'";
    case TokenType::SLASH:
        return "'/'";
    case TokenType::PERCENT:
        return "'%'";
    case TokenType::LT:
        return "'<'";
    case TokenType::GT:
        return "'>'";
    case TokenType::LE:
        return "'<='";
    case TokenType::GE:
        return "'>='";
    case TokenType::EQ:
        return "'=='";
    case TokenType::NE:
        return "'!='";
    case TokenType::ASSIGN:
        return "'='";
    case TokenType::LOGIC_AND:
        return "'&&'";
    case TokenType::LOGIC_OR:
        return "'||'";
    case TokenType::LOGIC_NOT:
        return "'!'";
    case TokenType::IDENT:
        return "Ident";
    case TokenType::INT_CONST:
        return "IntConst";
    default:
        return "Unknown";
    }
}

Lexer::Lexer(std::istream &in) : input(in), pos(0)
{
    buffer = std::string((std::istreambuf_iterator<char>(input)),
                         std::istreambuf_iterator<char>());
}

char Lexer::peek(int offset)
{
    if (pos + offset >= buffer.size())
        return '\0';
    return buffer[pos + offset];
}

char Lexer::advance()
{
    return buffer[pos++];
}

bool Lexer::match(char c)
{
    if (peek() == c)
    {
        advance();
        return true;
    }
    return false;
}

bool Lexer::isDigit(char c)
{
    return c >= '0' && c <= '9';
}

bool Lexer::isAlpha(char c)
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

bool Lexer::isAlphaNumeric(char c)
{
    return isAlpha(c) || isDigit(c);
}

void Lexer::skipWhitespaceAndComments()
{
    while (true)
    {
        char c = peek();
        if (c == ' ' || c == '\t' || c == '\n' || c == '\r')
        {
            advance();
        }
        else if (c == '/')
        {
            if (peek(1) == '/')
            {
                // 单行注释
                advance();
                advance();
                while (peek() != '\0' && peek() != '\n')
                    advance();
            }
            else if (peek(1) == '*')
            {
                // 多行注释
                advance();
                advance();
                while (peek() != '\0' && !(peek() == '*' && peek(1) == '/'))
                {
                    advance();
                }
                if (peek() == '\0')
                {
                    // 注释未闭合，按题目可假设输入合法
                    break;
                }
                advance();
                advance(); // 跳过 */
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

Token Lexer::readNumber()
{
    std::string num;
    if (peek() == '-')
    {
        num += advance();
    }
    while (isDigit(peek()))
    {
        num += advance();
    }
    // 检查是否为 0 或非零开头
    if (num == "-")
    {
        // 非法，但题目假设输入合法，可忽略
    }
    else if (num.size() > 1 && num[num[0] == '-' ? 1 : 0] == '0')
    {
        // 如 "01" 非法，但题目说 NUMBER 是 -?(0|[1-9][0-9]*)，所以合法输入不会有前导0
        // 我们按合法输入处理
    }
    return {TokenType::INT_CONST, num};
}

Token Lexer::readIdentifierOrKeyword()
{
    std::string ident;
    while (isAlphaNumeric(peek()))
    {
        ident += advance();
    }

    static const std::unordered_map<std::string, TokenType> keywords = {
        {"int", TokenType::KW_INT},
        {"void", TokenType::KW_VOID},
        {"if", TokenType::KW_IF},
        {"else", TokenType::KW_ELSE},
        {"while", TokenType::KW_WHILE},
        {"break", TokenType::KW_BREAK},
        {"continue", TokenType::KW_CONTINUE},
        {"return", TokenType::KW_RETURN}};

    auto it = keywords.find(ident);
    if (it != keywords.end())
    {
        return {it->second, ident};
    }
    else
    {
        return {TokenType::IDENT, ident};
    }
}

std::vector<Token> Lexer::tokenize()
{
    std::vector<Token> tokens;
    while (peek() != '\0')
    {
        skipWhitespaceAndComments();
        if (peek() == '\0')
            break;

        char c = peek();

        // 处理单字符 token
        switch (c)
        {
        case '(':
            advance();
            tokens.push_back({TokenType::LPAREN, "("});
            continue;
        case ')':
            advance();
            tokens.push_back({TokenType::RPAREN, ")"});
            continue;
        case '{':
            advance();
            tokens.push_back({TokenType::LBRACE, "{"});
            continue;
        case '}':
            advance();
            tokens.push_back({TokenType::RBRACE, "}"});
            continue;
        case ';':
            advance();
            tokens.push_back({TokenType::SEMICOLON, ";"});
            continue;
        case ',':
            advance();
            tokens.push_back({TokenType::COMMA, ","});
            continue;
        case '+':
            advance();
            tokens.push_back({TokenType::PLUS, "+"});
            continue;
        case '*':
            advance();
            tokens.push_back({TokenType::STAR, "*"});
            continue;
        case '%':
            advance();
            tokens.push_back({TokenType::PERCENT, "%"});
            continue;
        case '=':
            advance();
            if (peek() == '=')
            {
                advance();
                tokens.push_back({TokenType::EQ, "=="});
            }
            else
            {
                tokens.push_back({TokenType::ASSIGN, "="});
            }
            continue;
        case '!':
            advance();
            if (peek() == '=')
            {
                advance();
                tokens.push_back({TokenType::NE, "!="});
            }
            else
            {
                tokens.push_back({TokenType::LOGIC_NOT, "!"});
            }
            continue;
        case '<':
            advance();
            if (peek() == '=')
            {
                advance();
                tokens.push_back({TokenType::LE, "<="});
            }
            else
            {
                tokens.push_back({TokenType::LT, "<"});
            }
            continue;
        case '>':
            advance();
            if (peek() == '=')
            {
                advance();
                tokens.push_back({TokenType::GE, ">="});
            }
            else
            {
                tokens.push_back({TokenType::GT, ">"});
            }
            continue;
        case '&':
            advance();
            if (peek() == '&')
            {
                advance();
                tokens.push_back({TokenType::LOGIC_AND, "&&"});
            }
            else
            {
                // 非法，但题目输入合法，可报错或忽略
                // 这里假设输入合法，不会单独出现 &
            }
            continue;
        case '|':
            advance();
            if (peek() == '|')
            {
                advance();
                tokens.push_back({TokenType::LOGIC_OR, "||"});
            }
            else
            {
                // 同上
            }
            continue;
        case '/':
            // 此时不可能是注释（已被 skipWhitespaceAndComments 处理）
            // 所以是除法
            advance();
            tokens.push_back({TokenType::SLASH, "/"});
            continue;
        case '-':
            if (isDigit(peek(1)))
            {
                tokens.push_back(readNumber());
            }
            else
            {
                advance();
                tokens.push_back({TokenType::MINUS, "-"});
            }
            continue;
        }

        if (isDigit(c))
        {
            tokens.push_back(readNumber());
            continue;
        }

        if (isAlpha(c))
        {
            tokens.push_back(readIdentifierOrKeyword());
            continue;
        }

        // 遇到非法字符（题目假设输入合法，可忽略或报错）
        // 为安全起见，可以抛异常
        throw std::runtime_error("Unexpected character: " + std::string(1, c));
    }
    return tokens;
}