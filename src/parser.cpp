#include "parser.h"
#include <iostream>
#include <unordered_set>

Parser::Parser(const std::vector<Token> &tokens)
    : tokens_(tokens), current_(0) {}

bool Parser::parse()
{
    parseCompUnit();
    return errors_.empty();
}

const Token &Parser::currentToken() const
{
    if (current_ >= tokens_.size())
    {
        static Token endToken{0, TokenType::UNKNOWN, "", -1, -1};
        return endToken;
    }
    return tokens_[current_];
}

const Token &Parser::peek(int offset) const
{
    static Token endToken{0, TokenType::UNKNOWN, "", -1, -1};
    if (current_ + offset >= tokens_.size())
    {
        return endToken;
    }
    return tokens_[current_ + offset];
}

bool Parser::isAtEnd() const
{
    return current_ >= tokens_.size();
}

void Parser::advance()
{
    if (!isAtEnd())
    {
        current_++;
    }
}

bool Parser::check(TokenType type) const
{
    if (isAtEnd())
        return false;
    return currentToken().type == type;
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

bool Parser::consume(TokenType type, const std::string &errorMsg)
{
    if (match(type))
    {
        return true;
    }

    reportError(currentToken());
    return false;
}

void Parser::synchronize()
{
    // 同步到下一个语句或声明的开始
    while (!isAtEnd())
    {
        // 函数定义开始
        if ((check(TokenType::KEYWORD_INT) || check(TokenType::KEYWORD_VOID)) &&
            peek().type == TokenType::IDENTIFIER &&
            peek(2).type == TokenType::LEFT_PAREN)
        {
            return;
        }

        // 语句开始
        if (check(TokenType::SEMICOLON) ||
            check(TokenType::LEFT_BRACE) ||
            check(TokenType::RIGHT_BRACE) ||
            check(TokenType::KEYWORD_IF) ||
            check(TokenType::KEYWORD_WHILE) ||
            check(TokenType::KEYWORD_RETURN) ||
            check(TokenType::KEYWORD_BREAK) ||
            check(TokenType::KEYWORD_CONTINUE) ||
            (check(TokenType::IDENTIFIER) &&
             (peek().type == TokenType::ASSIGN || peek().type == TokenType::LEFT_PAREN || peek().type == TokenType::SEMICOLON)) ||
            check(TokenType::KEYWORD_INT))
        {
            return;
        }

        advance();
    }
}

void Parser::reportError(const Token &token)
{
    // 避免重复报告同一行的错误
    if (reportedLines_.find(token.line) != reportedLines_.end())
    {
        return;
    }

    errors_.push_back(token.line);
    reportedLines_.insert(token.line);
    synchronize();
}

// CompUnit → FuncDef+
void Parser::parseCompUnit()
{
    if (isAtEnd())
    {
        reportError(currentToken());
        return;
    }

    bool hasMain = false;
    while (!isAtEnd() && (check(TokenType::KEYWORD_INT) || check(TokenType::KEYWORD_VOID)))
    {
        int funcLine = currentToken().line;
        parseFuncDef();

        // 检查是否是main函数
        if (funcLine == currentToken().line - 1)
        {
            // 可能是main函数
            for (int i = current_ - 1; i >= 0 && i > current_ - 10; i--)
            {
                if (tokens_[i].type == TokenType::IDENTIFIER && tokens_[i].value == "main")
                {
                    hasMain = true;
                    break;
                }
            }
        }
    }

    // 检查是否还有未处理的token
    if (!isAtEnd())
    {
        reportError(currentToken());
    }
}

// FuncDef → ("int" | "void") ID "(" (Param ("," Param)*)? ")" Block
void Parser::parseFuncDef()
{
    // 返回类型
    if (!match(TokenType::KEYWORD_INT) && !match(TokenType::KEYWORD_VOID))
    {
        reportError(currentToken());
        synchronize();
        return;
    }

    // 函数名
    if (!match(TokenType::IDENTIFIER))
    {
        reportError(currentToken());
        synchronize();
        return;
    }

    // 左括号
    if (!match(TokenType::LEFT_PAREN))
    {
        reportError(currentToken());
        // 尝试同步到右括号或左大括号
        while (!isAtEnd() && !check(TokenType::RIGHT_PAREN) && !check(TokenType::LEFT_BRACE))
        {
            advance();
        }
    }

    // 参数列表（可选）
    if (check(TokenType::KEYWORD_INT))
    {
        parseParam();

        // 更多参数
        while (match(TokenType::COMMA))
        {
            if (check(TokenType::RIGHT_PAREN))
            {
                // 逗号后直接是右括号，参数缺失
                reportError(currentToken());
                break;
            }
            parseParam();
        }
    }

    // 右括号
    if (!match(TokenType::RIGHT_PAREN))
    {
        reportError(currentToken());
        // 尝试同步到左大括号
        while (!isAtEnd() && !check(TokenType::LEFT_BRACE))
        {
            advance();
        }
    }

    // 函数体
    if (check(TokenType::LEFT_BRACE))
    {
        parseBlock();
    }
    else
    {
        reportError(currentToken());
        synchronize();
    }
}

// Param → "int" ID
void Parser::parseParam()
{
    if (!match(TokenType::KEYWORD_INT))
    {
        reportError(currentToken());
        return;
    }
    if (!match(TokenType::IDENTIFIER))
    {
        reportError(currentToken());
    }
}

// Block → "{" Stmt* "}"
void Parser::parseBlock()
{
    if (!match(TokenType::LEFT_BRACE))
    {
        reportError(currentToken());
        return;
    }

    while (!isAtEnd() && !check(TokenType::RIGHT_BRACE))
    {
        parseStmt();
    }

    if (!match(TokenType::RIGHT_BRACE))
    {
        reportError(currentToken());
        // 尝试同步到下一个函数定义
        synchronize();
    }
}

// Stmt → Block | ";" | Expr ";" | ID "=" Expr ";"
//        | "int" ID "=" Expr ";"
//        | "if" "(" Expr ")" Stmt ("else" Stmt)?
//        | "while" "(" Expr ")" Stmt
//        | "break" ";" | "continue" ";" | "return" Expr ";"
void Parser::parseStmt()
{
    if (match(TokenType::LEFT_BRACE))
    {
        // Block
        parseBlock();
    }
    else if (match(TokenType::SEMICOLON))
    {
        // 空语句
        return;
    }
    else if (match(TokenType::KEYWORD_INT))
    {
        // 变量声明: "int" ID "=" Expr ";"
        if (!match(TokenType::IDENTIFIER))
        {
            reportError(currentToken());
            synchronize();
            return;
        }
        if (!match(TokenType::ASSIGN))
        {
            reportError(currentToken());
            synchronize();
            return;
        }
        parseExpr();
        if (!match(TokenType::SEMICOLON))
        {
            reportError(currentToken());
            synchronize();
        }
    }
    else if (match(TokenType::KEYWORD_IF))
    {
        // if语句
        if (!match(TokenType::LEFT_PAREN))
        {
            reportError(currentToken());
        }
        parseExpr();
        if (!match(TokenType::RIGHT_PAREN))
        {
            reportError(currentToken());
        }
        parseStmt();

        if (match(TokenType::KEYWORD_ELSE))
        {
            parseStmt();
        }
    }
    else if (match(TokenType::KEYWORD_WHILE))
    {
        // while语句
        if (!match(TokenType::LEFT_PAREN))
        {
            reportError(currentToken());
        }
        parseExpr();
        if (!match(TokenType::RIGHT_PAREN))
        {
            reportError(currentToken());
        }
        parseStmt();
    }
    else if (match(TokenType::KEYWORD_BREAK))
    {
        if (!match(TokenType::SEMICOLON))
        {
            reportError(currentToken());
            synchronize();
        }
    }
    else if (match(TokenType::KEYWORD_CONTINUE))
    {
        if (!match(TokenType::SEMICOLON))
        {
            reportError(currentToken());
            synchronize();
        }
    }
    else if (match(TokenType::KEYWORD_RETURN))
    {
        // 检查是否有表达式
        if (!check(TokenType::SEMICOLON))
        {
            parseExpr();
        }
        if (!match(TokenType::SEMICOLON))
        {
            reportError(currentToken());
            synchronize();
        }
    }
    else if (check(TokenType::IDENTIFIER) && peek().type == TokenType::ASSIGN)
    {
        // 赋值语句: ID "=" Expr ";"
        advance(); // 消费ID
        advance(); // 消费"="
        parseExpr();
        if (!match(TokenType::SEMICOLON))
        {
            reportError(currentToken());
            synchronize();
        }
    }
    else if (check(TokenType::IDENTIFIER) && peek().type == TokenType::LEFT_PAREN)
    {
        // 函数调用: ID "(" ... ")"
        parseExpr();
        if (!match(TokenType::SEMICOLON))
        {
            reportError(currentToken());
            synchronize();
        }
    }
    else
    {
        // 表达式语句: Expr ";"
        parseExpr();
        if (!match(TokenType::SEMICOLON))
        {
            reportError(currentToken());
            synchronize();
        }
    }
}

void Parser::parseExpr()
{
    parseLOrExpr();
}

// LOrExpr → LAndExpr | LOrExpr "||" LAndExpr
void Parser::parseLOrExpr()
{
    parseLAndExpr();
    while (match(TokenType::OR))
    {
        parseLAndExpr();
    }
}

// LAndExpr → RelExpr | LAndExpr "&&" RelExpr
void Parser::parseLAndExpr()
{
    parseRelExpr();
    while (match(TokenType::AND))
    {
        parseRelExpr();
    }
}

// RelExpr → AddExpr | RelExpr ("<" | ">" | "<=" | ">=" | "==" | "!=") AddExpr
void Parser::parseRelExpr()
{
    parseAddExpr();
    while (check(TokenType::LESS) || check(TokenType::GREATER) ||
           check(TokenType::LESS_EQUAL) || check(TokenType::GREATER_EQUAL) ||
           check(TokenType::EQUALS) || check(TokenType::NOT_EQUALS))
    {
        advance(); // 消费运算符
        parseAddExpr();
    }
}

// AddExpr → MulExpr | AddExpr ("+" | "-") MulExpr
void Parser::parseAddExpr()
{
    parseMulExpr();
    while (check(TokenType::PLUS) || check(TokenType::MINUS))
    {
        advance(); // 消费运算符
        parseMulExpr();
    }
}

// MulExpr → UnaryExpr | MulExpr ("*" | "/" | "%") UnaryExpr
void Parser::parseMulExpr()
{
    parseUnaryExpr();
    while (check(TokenType::MULTIPLY) || check(TokenType::DIVIDE) || check(TokenType::MODULO))
    {
        advance(); // 消费运算符
        parseUnaryExpr();
    }
}

// UnaryExpr → PrimaryExpr | ("+" | "-" | "!") UnaryExpr
void Parser::parseUnaryExpr()
{
    if (match(TokenType::PLUS) || match(TokenType::MINUS) || match(TokenType::NOT))
    {
        parseUnaryExpr();
    }
    else
    {
        parsePrimaryExpr();
    }
}

// PrimaryExpr → ID | NUMBER | "(" Expr ")" | ID "(" (Expr ("," Expr)*)? ")"
void Parser::parsePrimaryExpr()
{
    if (match(TokenType::IDENTIFIER))
    {
        // 可能是函数调用或普通标识符
        if (match(TokenType::LEFT_PAREN))
        {
            // 函数调用
            if (!check(TokenType::RIGHT_PAREN))
            {
                parseExpr();
                while (match(TokenType::COMMA))
                {
                    if (check(TokenType::RIGHT_PAREN))
                    {
                        // 逗号后直接是右括号，参数缺失
                        reportError(currentToken());
                        break;
                    }
                    parseExpr();
                }
            }
            if (!match(TokenType::RIGHT_PAREN))
            {
                reportError(currentToken());
                synchronize();
            }
        }
        // 否则就是普通标识符，已经消费过了
    }
    else if (match(TokenType::INT_CONST))
    {
        // 整数常量，已经消费过了
    }
    else if (match(TokenType::LEFT_PAREN))
    {
        parseExpr();
        if (!match(TokenType::RIGHT_PAREN))
        {
            reportError(currentToken());
            synchronize();
        }
    }
    else
    {
        reportError(currentToken());
        synchronize();
    }
}