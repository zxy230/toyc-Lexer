
#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>

using namespace std;

enum class TokenType
{
    INT,
    VOID,
    IF,
    ELSE,
    WHILE,
    BREAK,
    CONTINUE,
    RETURN,
    IDENTIFIER,
    INTCONST,
    PLUS,
    MINUS,
    MULTIPLY,
    DIVIDE,
    MODULO,
    ASSIGN,
    EQUAL,
    NOT_EQUAL,
    LESS,
    LESS_EQUAL,
    GREATER,
    GREATER_EQUAL,
    AND,
    OR,
    NOT,
    LEFT_PAREN,
    RIGHT_PAREN,
    LEFT_BRACE,
    RIGHT_BRACE,
    SEMICOLON,
    COMMA,
    END_OF_FILE,
    UNKNOWN
};

struct Token
{
    TokenType type;
    string value;
    int index;
    int line;

    Token() : type(TokenType::END_OF_FILE), value(""), index(0), line(1) {}
    Token(TokenType t, const string &v, int idx, int l = 1)
        : type(t), value(v), index(idx), line(l) {}
};

class Lexer
{
private:
    string m_source;
    size_t m_pos;
    int m_tokenIdCounter;
    int m_currentLine;
    unordered_map<string, TokenType> m_keywords;

    char currentChar() const;
    char peekNextChar() const;
    void advance();
    bool isAtEnd() const;
    void skipWhitespace();
    void skipComments();
    Token consumeNumber();
    Token consumeIdentifier();
    Token consumeOperator();
    Token makeToken(TokenType type, const string &text, int startLine);
    string typeToStr(TokenType t) const;

public:
    explicit Lexer(string source);
    Token nextToken();
    vector<Token> getAllTokens();
    void output();
};

#endif