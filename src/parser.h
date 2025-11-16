#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include <vector>
#include <string>
#include <set>
#include <stdexcept>
#include <unordered_set>

struct ErrorInfo
{
    int line;
    std::string message;
    ErrorInfo(int l, const std::string &m) : line(l), message(m) {}
};

class Parser
{
private:
    std::vector<Token> m_tokens;
    size_t m_index;

    std::vector<ErrorInfo> m_errors;
    bool hasMain;
    std::unordered_set<std::string> functionNames;

    class ParseError : public std::runtime_error
    {
    public:
        ParseError() : std::runtime_error("Parse Error") {}
    };

    Token current() const;
    Token previous() const;
    bool isAtEnd() const;
    void advance();
    bool check(TokenType type) const;
    bool match(TokenType type);

    void reportError(const std::string &msg);
    void requireToken(TokenType type, const std::string &expected);

    void recoverToStatement();
    void recoverToFunction();

    void analyzeProgram();
    void analyzeFunction();
    void analyzeParameters();
    void analyzeSingleParam();
    void analyzeCodeBlock();
    void analyzeStatement();
    void analyzeVariableDeclaration();
    void analyzeAssignmentOrCall();
    void analyzeConditional();
    void analyzeLoop();
    void analyzeJumpStatement();
    void analyzeFunctionArguments();
    void analyzeExpression();
    void analyzeLogicalOr();
    void analyzeLogicalAnd();
    void analyzeComparison();
    void analyzeAddition();
    void analyzeMultiplication();
    void analyzeUnary();
    void analyzePrimary();

public:
    explicit Parser(std::vector<Token> tokens);
    bool parse();
    void printErrors();
};

#endif