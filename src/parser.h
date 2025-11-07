#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include <vector>
#include <string>
#include <unordered_set>

// 语法分析器类
class Parser
{
public:
    Parser(const std::vector<Token> &tokens);
    bool parse();
    const std::vector<int> &getErrors() const { return errors_; }

private:
    std::vector<Token> tokens_;
    size_t current_;
    std::vector<int> errors_;
    std::unordered_set<int> reportedLines_;

    // 辅助方法
    const Token &currentToken() const;
    const Token &peek(int offset = 1) const;
    bool isAtEnd() const;
    void advance();
    bool check(TokenType type) const;
    bool match(TokenType type);
    bool consume(TokenType type, const std::string &errorMsg = "");
    void synchronize();
    void reportError(const Token &token);

    // 递归下降解析方法
    void parseCompUnit();
    void parseFuncDef();
    void parseParam();
    void parseBlock();
    void parseStmt();
    void parseExpr();
    void parseLOrExpr();
    void parseLAndExpr();
    void parseRelExpr();
    void parseAddExpr();
    void parseMulExpr();
    void parseUnaryExpr();
    void parsePrimaryExpr();
};

#endif