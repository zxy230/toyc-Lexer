// src/Lexer.h
#pragma once
#include <string>
#include <vector>
#include <iostream>

enum class TokenType
{
    KW_INT,
    KW_VOID,
    KW_IF,
    KW_ELSE,
    KW_WHILE,
    KW_BREAK,
    KW_CONTINUE,
    KW_RETURN,
    LPAREN,
    RPAREN,
    LBRACE,
    RBRACE,
    SEMICOLON,
    COMMA,
    PLUS,
    MINUS,
    STAR,
    SLASH,
    PERCENT,
    LT,
    GT,
    LE,
    GE,
    EQ,
    NE,
    ASSIGN,
    LOGIC_AND,
    LOGIC_OR,
    LOGIC_NOT,
    IDENT,
    INT_CONST,
    END // 表示输入结束
};

struct Token
{
    TokenType type;
    std::string lexeme;
};

std::string tokenTypeToString(TokenType type);

class Lexer
{
public:
    explicit Lexer(std::istream &input);
    std::vector<Token> tokenize();

private:
    std::istream &input;
    std::string buffer;
    size_t pos;

    char peek(int offset = 0);
    char advance();
    bool match(char c);
    void skipWhitespaceAndComments();
    Token readNumber();
    Token readIdentifierOrKeyword();
    bool isDigit(char c);
    bool isAlpha(char c);
    bool isAlphaNumeric(char c);
};