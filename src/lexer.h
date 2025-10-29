#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>

// Token 类型定义
enum class TokenType
{
    // 关键字
    KEYWORD_INT,
    KEYWORD_VOID,
    KEYWORD_IF,
    KEYWORD_ELSE,
    KEYWORD_WHILE,
    KEYWORD_BREAK,
    KEYWORD_CONTINUE,
    KEYWORD_RETURN,

    // 标识符和常量
    IDENTIFIER,
    INT_CONST,

    // 运算符
    PLUS,          // +
    MINUS,         // -
    MULTIPLY,      // *
    DIVIDE,        // /
    MODULO,        // %
    ASSIGN,        // =
    EQUALS,        // ==
    NOT_EQUALS,    // !=
    LESS,          // <
    GREATER,       // >
    LESS_EQUAL,    // <=
    GREATER_EQUAL, // >=
    AND,           // &&
    OR,            // ||
    NOT,           // !

    // 分隔符
    LEFT_PAREN,  // (
    RIGHT_PAREN, // )
    LEFT_BRACE,  // {
    RIGHT_BRACE, // }
    SEMICOLON,   // ;
    COMMA,       // ,

    UNKNOWN
};

// Token结构体
struct Token
{
    int index;
    TokenType type;
    std::string value;
    int line;
    int col;
};

// 词法分析器类
class Lexer
{
public:
    Lexer(const std::string &input);
    std::vector<Token> tokenize();

private:
    std::string input;
    size_t pos; // 当前位置
    int line;   // 当前行号
    int col;    // 当前列号

    Token readNextToken();
    Token readIdentifier();
    Token readNumber();
    Token readOperator();

    // 字符处理和查询
    char current() const;
    char peek(int offset = 1) const;
    void advance();
    bool match(char expected);
    bool isAtEnd() const;

    // 字符分类
    static bool isIdentifierStart(char c);
    static bool isIdentifierPart(char c);
    static bool isDigit(char c);

    // 跳过无关内容
    void skipWhitespace();
};

std::string tokenTypeToString(TokenType type);

#endif