# ToyC 词法分析器
使用 C++ 编写的词法分析器，用于解析 ToyC 语言。

# 构建与运行

## 构建镜像
docker build -t toy-lexer .
**每次修改源码后都需重新构建镜像**

## 快速测试单行代码
echo 'int a = 1;' | docker run -i toy-lexer

## 管道测试
cat sample.c | docker run -i toy-lexer

## 重定向测试
docker run -i toy-lexer < sample.c

mkdir -p build
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build

mkdir build
cd build 
cmake ..
make



// #include "parser.h"
// #include <iostream>
// #include <string>
// #include <sstream>

// int main()
// {
//     std::string input;
//     std::string line;

//     while (std::getline(std::cin, line))
//     {
//         input += line + "\n";
//     }

//     Parser parser(input);
//     parser.parse();
//     parser.printErrors();

//     return 0;
// }


// #include "parser.h"
// #include <iostream>
// #include <algorithm>

// Parser::Parser(const std::string &input) : lexer(input), hasMain(false)
// {
//     current = lexer.nextToken();
//     if (current.type == UNKNOWN)
//         reportError("Lexical error");
// }

// void Parser::consumeToken()
// {
//     current = lexer.nextToken();
// }

// bool Parser::tryConsume(TokenType type)
// {
//     if (lookAhead(type))
//     {
//         consumeToken();
//         return true;
//     }
//     return false;
// }

// bool Parser::lookAhead(TokenType type) const
// {
//     return current.type == type;
// }

// void Parser::reportError(const std::string &msg)
// {
//     const int ln = current.line;

//     bool exists = std::any_of(errors.begin(), errors.end(),
//                               [ln](const ErrorInfo &e)
//                               { return e.line == ln; });

//     if (!exists)
//         errors.emplace_back(ln, msg);
// }

// void Parser::expectToken(const std::string &expected)
// {
//     reportError("Expected " + expected);
// }

// bool Parser::isStatementEnd() const
// {
//     return lookAhead(SEMICOLON) || lookAhead(RIGHT_PAREN) ||
//            lookAhead(RIGHT_BRACE) || lookAhead(COMMA) || lookAhead(END_OF_FILE);
// }

// void Parser::recoverToCloseParen()
// {
//     while (!lookAhead(RIGHT_PAREN) && !lookAhead(END_OF_FILE) && !lookAhead(LEFT_BRACE))
//         consumeToken();
// }

// void Parser::analyzeProgram()
// {
//     if (lookAhead(END_OF_FILE))
//     {
//         reportError("Empty program");
//         return;
//     }

//     while (!lookAhead(END_OF_FILE))
//     {
//         const size_t errCnt = errors.size();
//         const int tokIdx = current.index;

//         analyzeFunction();

//         const size_t newErrCnt = errors.size();
//         const int newTokIdx = current.index;

//         if (newErrCnt > errCnt)
//         {
//             if (tokIdx == newTokIdx || lookAhead(LEFT_BRACE))
//                 break;

//             while (!lookAhead(END_OF_FILE) && !lookAhead(INT) && !lookAhead(VOID))
//                 consumeToken();
//         }
//         else if (tokIdx == newTokIdx && !lookAhead(END_OF_FILE))
//         {
//             consumeToken();
//         }

//         if (lookAhead(END_OF_FILE))
//             break;
//     }

//     if (!hasMain)
//         reportError("Missing main function");
// }

// void Parser::analyzeFunction()
// {
//     if (!lookAhead(INT) && !lookAhead(VOID))
//     {
//         expectToken("int or void");
//         return;
//     }
//     consumeToken();

//     if (!lookAhead(IDENTIFIER))
//     {
//         expectToken("function name");
//         return;
//     }

//     const std::string name = current.value;
//     consumeToken();

//     if (name == "main")
//         hasMain = true;

//     if (functionNames.count(name))
//         reportError("Duplicate function name");
//     else
//         functionNames.insert(name);

//     if (!tryConsume(LEFT_PAREN))
//     {
//         expectToken("(");
//         if (lookAhead(RIGHT_PAREN))
//             consumeToken();
//         return;
//     }

//     analyzeParameters();

//     if (!tryConsume(RIGHT_PAREN))
//     {
//         expectToken(")");
//         return;
//     }

//     analyzeCodeBlock();
// }

// void Parser::analyzeParameters()
// {
//     if (lookAhead(COMMA))
//     {
//         expectToken("int");
//         recoverToCloseParen();
//         return;
//     }

//     if (!lookAhead(INT))
//         return;

//     const size_t cnt = errors.size();
//     analyzeSingleParam();

//     if (errors.size() > cnt)
//     {
//         recoverToCloseParen();
//         return;
//     }

//     while (tryConsume(COMMA))
//     {
//         if (lookAhead(RIGHT_PAREN))
//             break;

//         const size_t cnt2 = errors.size();
//         analyzeSingleParam();

//         if (errors.size() > cnt2)
//             break;
//     }
// }

// void Parser::analyzeSingleParam()
// {
//     if (!tryConsume(INT))
//     {
//         expectToken("int");
//         return;
//     }

//     if (!tryConsume(IDENTIFIER))
//         expectToken("parameter name");
// }

// void Parser::analyzeCodeBlock()
// {
//     if (!tryConsume(LEFT_BRACE))
//     {
//         expectToken("{");
//         return;
//     }

//     while (!lookAhead(RIGHT_BRACE) && !lookAhead(END_OF_FILE))
//     {
//         const int idx = current.index;
//         analyzeStatement();
//         const int newIdx = current.index;

//         if (idx == newIdx && !lookAhead(RIGHT_BRACE) && !lookAhead(END_OF_FILE))
//             consumeToken();

//         if (!errors.empty() && lookAhead(END_OF_FILE))
//             break;
//     }

//     if (!tryConsume(RIGHT_BRACE))
//         expectToken("}");
// }

// void Parser::analyzeStatement()
// {
//     if (lookAhead(LEFT_BRACE))
//     {
//         analyzeCodeBlock();
//     }
//     else if (tryConsume(SEMICOLON))
//     {
//         return;
//     }
//     else if (lookAhead(INT))
//     {
//         analyzeVariableDeclaration();
//     }
//     else if (lookAhead(IDENTIFIER))
//     {
//         analyzeAssignmentOrCall();
//     }
//     else if (lookAhead(IF))
//     {
//         analyzeConditional();
//     }
//     else if (lookAhead(WHILE))
//     {
//         analyzeLoop();
//     }
//     else if (lookAhead(BREAK) || lookAhead(CONTINUE))
//     {
//         analyzeJumpStatement();
//     }
//     else if (lookAhead(RETURN))
//     {
//         consumeToken();
//         analyzeExpression();
//         if (!tryConsume(SEMICOLON))
//             expectToken(";");
//     }
//     else if (lookAhead(ELSE))
//     {
//         consumeToken();
//     }
//     else
//     {
//         analyzeExpression();
//         if (!tryConsume(SEMICOLON))
//             expectToken(";");
//     }
// }

// // void Parser::analyzeVariableDeclaration()
// // {
// //     consumeToken();

// //     if (!tryConsume(IDENTIFIER))
// //     {
// //         expectToken("variable name");
// //         return;
// //     }

// //     if (!tryConsume(ASSIGN))
// //     {
// //         expectToken("=");
// //         return;
// //     }

// //     analyzeExpression();

// //     if (!tryConsume(SEMICOLON))
// //         expectToken(";");
// // }
// void Parser::analyzeVariableDeclaration()
// {
//     consumeToken(); // Consume 'int' keyword

//     do
//     {
//         // Parse variable name (identifier required)
//         if (!tryConsume(IDENTIFIER))
//         {
//             expectToken("variable name");
//             while (!lookAhead(COMMA) && !lookAhead(SEMICOLON) && !lookAhead(END_OF_FILE))
//                 consumeToken();
//             if (tryConsume(COMMA))
//                 continue;
//             else
//                 break;
//         }

//         // Optional assignment: parse expr only if '=' exists
//         if (tryConsume(ASSIGN))
//         {
//             if (isStatementEnd())
//             {
//                 reportError("Missing expression after '='");
//                 while (!lookAhead(COMMA) && !lookAhead(SEMICOLON) && !lookAhead(END_OF_FILE))
//                     consumeToken();
//                 if (tryConsume(COMMA))
//                     continue;
//                 else
//                     break;
//             }
//             analyzeExpression();
//         }

//         if (!lookAhead(COMMA))
//             break; // No more variables

//         consumeToken(); // Consume ','

//         // Reject trailing comma
//         if (lookAhead(SEMICOLON) || lookAhead(END_OF_FILE))
//         {
//             reportError("Missing variable name after ','");
//             break;
//         }

//     } while (true);

//     // Require semicolon to end declaration
//     if (!tryConsume(SEMICOLON))
//     {
//         expectToken(";");
//     }
// }

// void Parser::analyzeAssignmentOrCall()
// {
//     consumeToken();

//     if (lookAhead(ASSIGN))
//     {
//         consumeToken();
//         analyzeExpression();
//         if (!tryConsume(SEMICOLON))
//             expectToken(";");
//     }
//     else if (lookAhead(LEFT_PAREN))
//     {
//         consumeToken();
//         analyzeFunctionArguments();
//         if (!tryConsume(RIGHT_PAREN))
//             expectToken(")");
//         if (!tryConsume(SEMICOLON))
//             expectToken(";");
//     }
//     else
//     {
//         reportError("Invalid statement");
//     }
// }

// void Parser::analyzeConditional()
// {
//     consumeToken();

//     if (!tryConsume(LEFT_PAREN))
//     {
//         expectToken("(");
//         return;
//     }

//     analyzeExpression();

//     if (!tryConsume(RIGHT_PAREN))
//     {
//         expectToken(")");
//         return;
//     }

//     analyzeStatement();

//     if (tryConsume(ELSE))
//         analyzeStatement();
// }

// void Parser::analyzeLoop()
// {
//     consumeToken();

//     if (!tryConsume(LEFT_PAREN))
//     {
//         expectToken("(");
//         return;
//     }

//     analyzeExpression();

//     if (!tryConsume(RIGHT_PAREN))
//     {
//         expectToken(")");
//         return;
//     }

//     analyzeStatement();
// }

// void Parser::analyzeJumpStatement()
// {
//     consumeToken();

//     if (!tryConsume(SEMICOLON))
//         expectToken(";");
// }

// void Parser::analyzeFunctionArguments()
// {
//     if (lookAhead(RIGHT_PAREN))
//         return;

//     if (lookAhead(COMMA))
//     {
//         reportError("Missing argument");
//         return;
//     }

//     analyzeExpression();

//     while (tryConsume(COMMA))
//     {
//         if (isStatementEnd())
//         {
//             reportError("Missing argument");
//             break;
//         }
//         analyzeExpression();
//     }
// }

// void Parser::analyzeExpression()
// {
//     analyzeLogicalOr();
// }

// void Parser::analyzeLogicalOr()
// {
//     analyzeLogicalAnd();
//     while (tryConsume(OR))
//         analyzeLogicalAnd();
// }

// void Parser::analyzeLogicalAnd()
// {
//     analyzeComparison();
//     while (tryConsume(AND))
//         analyzeComparison();
// }

// void Parser::analyzeComparison()
// {
//     analyzeAddition();

//     static const TokenType ops[] = {LESS, GREATER, LESS_EQUAL, GREATER_EQUAL, EQUAL, NOT_EQUAL};

//     while (std::any_of(std::begin(ops), std::end(ops),
//                        [this](TokenType t)
//                        { return lookAhead(t); }))
//     {
//         consumeToken();
//         analyzeAddition();
//     }
// }

// void Parser::analyzeAddition()
// {
//     analyzeMultiplication();

//     while (lookAhead(PLUS) || lookAhead(MINUS))
//     {
//         consumeToken();
//         if (isStatementEnd())
//         {
//             reportError("Missing operand");
//             return;
//         }
//         analyzeMultiplication();
//     }
// }

// void Parser::analyzeMultiplication()
// {
//     analyzeUnary();

//     while (lookAhead(MULTIPLY) || lookAhead(DIVIDE) || lookAhead(MODULO))
//     {
//         consumeToken();
//         if (isStatementEnd())
//         {
//             reportError("Missing operand");
//             return;
//         }
//         analyzeUnary();
//     }
// }

// void Parser::analyzeUnary()
// {
//     if (lookAhead(PLUS) || lookAhead(MINUS) || lookAhead(NOT))
//     {
//         consumeToken();
//         if (isStatementEnd())
//         {
//             reportError("Missing operand");
//             return;
//         }
//         analyzeUnary();
//     }
//     else
//     {
//         analyzePrimary();
//     }
// }

// void Parser::analyzePrimary()
// {
//     if (lookAhead(IDENTIFIER))
//     {
//         consumeToken();
//         if (lookAhead(LEFT_PAREN))
//         {
//             consumeToken();
//             analyzeFunctionArguments();
//             if (!tryConsume(RIGHT_PAREN))
//                 expectToken(")");
//         }
//     }
//     else if (tryConsume(INTCONST))
//     {
//         return;
//     }
//     else if (tryConsume(LEFT_PAREN))
//     {
//         analyzeExpression();
//         if (!tryConsume(RIGHT_PAREN))
//             expectToken(")");
//     }
//     else
//     {
//         expectToken("expression");
//     }
// }

// bool Parser::parse()
// {
//     analyzeProgram();
//     return errors.empty();
// }

// void Parser::printErrors()
// {
//     if (errors.empty())
//     {
//         std::cout << "accept" << std::endl;
//         return;
//     }

//     std::cout << "reject" << std::endl;

//     std::set<int> printed;
//     for (const auto &e : errors)
//     {
//         if (printed.insert(e.line).second)
//             std::cout << e.line << std::endl;
//     }
// }

// #ifndef PARSER_H
// #define PARSER_H

// #include "lexer.h"
// #include <vector>
// #include <string>
// #include <set>

// struct ErrorInfo
// {
//     int line;
//     std::string message;
//     ErrorInfo(int l, const std::string &m) : line(l), message(m) {}
// };

// class Parser
// {
// private:
//     Lexer lexer;
//     Token current;
//     std::vector<ErrorInfo> errors;
//     bool hasMain;
//     std::set<std::string> functionNames;

//     void consumeToken();
//     bool tryConsume(TokenType type);
//     bool lookAhead(TokenType type) const;
//     void reportError(const std::string &msg);
//     void expectToken(const std::string &expected);
//     bool isStatementEnd() const;
//     void recoverToCloseParen();
//     void analyzeProgram();
//     void analyzeFunction();
//     void analyzeParameters();
//     void analyzeSingleParam();
//     void analyzeCodeBlock();
//     void analyzeStatement();
//     void analyzeVariableDeclaration();
//     void analyzeAssignmentOrCall();
//     void analyzeConditional();
//     void analyzeLoop();
//     void analyzeJumpStatement();
//     void analyzeFunctionArguments();
//     void analyzeExpression();
//     void analyzeLogicalOr();
//     void analyzeLogicalAnd();
//     void analyzeComparison();
//     void analyzeAddition();
//     void analyzeMultiplication();
//     void analyzeUnary();
//     void analyzePrimary();

// public:
//     explicit Parser(const std::string &input);
//     bool parse();
//     void printErrors();
// };

// #endif

// #ifndef LEXER_H
// #define LEXER_H

// #include <string>
// #include <vector>
// #include <map>
// #include <iostream>

// using namespace std;

// enum TokenType
// {
//     INT,
//     VOID,
//     IF,
//     ELSE,
//     WHILE,
//     BREAK,
//     CONTINUE,
//     RETURN,
//     IDENTIFIER,
//     INTCONST,
//     PLUS,
//     MINUS,
//     MULTIPLY,
//     DIVIDE,
//     MODULO,
//     ASSIGN,
//     EQUAL,
//     NOT_EQUAL,
//     LESS,
//     LESS_EQUAL,
//     GREATER,
//     GREATER_EQUAL,
//     AND,
//     OR,
//     NOT,
//     LEFT_PAREN,
//     RIGHT_PAREN,
//     LEFT_BRACE,
//     RIGHT_BRACE,
//     SEMICOLON,
//     COMMA,
//     END_OF_FILE,
//     UNKNOWN
// };

// struct Token
// {
//     TokenType type;
//     string value;
//     int index;
//     int line;

//     Token() : type(END_OF_FILE), value(""), index(0), line(1) {}
//     Token(TokenType t, const string &v, int idx, int l = 1)
//         : type(t), value(v), index(idx), line(l) {}
// };

// class Lexer
// {
// private:
//     string input;
//     int pos;
//     int tokenIndex;
//     int line;
//     map<string, TokenType> keywordMap;

//     char getChar();
//     char peek();
//     void next();
//     bool isAtEnd() const;
//     void skipSpace();
//     void skipComments();
//     Token readNumber();
//     Token readId();
//     Token readOp();
//     Token createToken(TokenType type, const string &val, int startLine);
//     string typeToStr(TokenType t);

// public:
//     explicit Lexer(string s);
//     Token nextToken();
//     vector<Token> getAllTokens();
//     void output();
// };

// #endif


// #include "lexer.h"
// #include <cctype>
// #include <algorithm>

// Lexer::Lexer(string s) : input(std::move(s)), pos(0), tokenIndex(0), line(1)
// {
//     static const std::pair<string, TokenType> kw[] = {
//         {"int", INT}, {"void", VOID}, {"if", IF}, {"else", ELSE}, {"while", WHILE}, {"break", BREAK}, {"continue", CONTINUE}, {"return", RETURN}};
//     keywordMap.insert(std::begin(kw), std::end(kw));
// }

// bool Lexer::isAtEnd() const
// {
//     return pos >= static_cast<int>(input.length());
// }

// char Lexer::getChar()
// {
//     return isAtEnd() ? '\0' : input[pos];
// }

// char Lexer::peek()
// {
//     return (pos + 1 >= static_cast<int>(input.length())) ? '\0' : input[pos + 1];
// }

// void Lexer::next()
// {
//     if (!isAtEnd())
//     {
//         if (input[pos] == '\n')
//             ++line;
//         ++pos;
//     }
// }

// void Lexer::skipSpace()
// {
//     while (!isAtEnd() && std::isspace(static_cast<unsigned char>(getChar())))
//         next();
// }

// void Lexer::skipComments()
// {
//     if (getChar() != '/' || (peek() != '/' && peek() != '*'))
//         return;

//     bool isLine = (peek() == '/');
//     next();
//     next();

//     if (isLine)
//     {
//         while (!isAtEnd() && getChar() != '\n')
//             next();
//     }
//     else
//     {
//         while (!isAtEnd() && !(getChar() == '*' && peek() == '/'))
//             next();
//         if (!isAtEnd())
//         {
//             next();
//             next();
//         }
//     }
// }

// Token Lexer::readNumber()
// {
//     const int ln = line;
//     string num;

//     if (isAtEnd() || !std::isdigit(static_cast<unsigned char>(getChar())))
//         return createToken(UNKNOWN, "", ln);

//     const char first = getChar();
//     num += first;
//     next();

//     if (first == '0')
//     {
//         if (!isAtEnd() && std::isdigit(static_cast<unsigned char>(getChar())))
//         {
//             while (!isAtEnd() && std::isdigit(static_cast<unsigned char>(getChar())))
//             {
//                 num += getChar();
//                 next();
//             }
//             return createToken(UNKNOWN, num, ln);
//         }
//         return createToken(INTCONST, num, ln);
//     }

//     while (!isAtEnd() && std::isdigit(static_cast<unsigned char>(getChar())))
//     {
//         num += getChar();
//         next();
//     }

//     return createToken(INTCONST, num, ln);
// }

// Token Lexer::readId()
// {
//     const int ln = line;
//     string id;

//     auto valid = [](char c)
//     {
//         return std::isalnum(static_cast<unsigned char>(c)) || c == '_';
//     };

//     while (!isAtEnd() && valid(getChar()))
//     {
//         id += getChar();
//         next();
//     }

//     auto it = keywordMap.find(id);
//     TokenType tp = (it != keywordMap.end()) ? it->second : IDENTIFIER;

//     return createToken(tp, id, ln);
// }

// Token Lexer::readOp()
// {
//     const int ln = line;
//     const char c1 = getChar();
//     next();

//     if (!isAtEnd())
//     {
//         const char c2 = getChar();
//         string dbl{c1, c2};

//         static const std::map<string, TokenType> doubles = {
//             {"==", EQUAL}, {"!=", NOT_EQUAL}, {"<=", LESS_EQUAL}, {">=", GREATER_EQUAL}, {"&&", AND}, {"||", OR}};

//         auto fd = doubles.find(dbl);
//         if (fd != doubles.end())
//         {
//             next();
//             return createToken(fd->second, dbl, ln);
//         }
//     }

//     static const std::map<char, TokenType> singles = {
//         {'+', PLUS}, {'-', MINUS}, {'*', MULTIPLY}, {'/', DIVIDE}, {'%', MODULO}, {'=', ASSIGN}, {'!', NOT}, {'<', LESS}, {'>', GREATER}, {'(', LEFT_PAREN}, {')', RIGHT_PAREN}, {'{', LEFT_BRACE}, {'}', RIGHT_BRACE}, {';', SEMICOLON}, {',', COMMA}};

//     auto it = singles.find(c1);
//     TokenType tp = (it != singles.end()) ? it->second : UNKNOWN;

//     return createToken(tp, string(1, c1), ln);
// }

// Token Lexer::nextToken()
// {
//     while (!isAtEnd())
//     {
//         skipSpace();
//         if (isAtEnd())
//             break;

//         const char c = getChar();
//         const char n = peek();

//         if (c == '/' && (n == '/' || n == '*'))
//         {
//             skipComments();
//             continue;
//         }

//         if (std::isdigit(static_cast<unsigned char>(c)))
//             return readNumber();

//         if (std::isalpha(static_cast<unsigned char>(c)) || c == '_')
//             return readId();

//         return readOp();
//     }

//     return createToken(END_OF_FILE, "", line);
// }

// vector<Token> Lexer::getAllTokens()
// {
//     vector<Token> toks;
//     Token t;

//     do
//     {
//         t = nextToken();
//         toks.push_back(t);
//     } while (t.type != END_OF_FILE);

//     return toks;
// }

// void Lexer::output()
// {
//     auto toks = getAllTokens();

//     for (const auto &t : toks)
//     {
//         if (t.type != END_OF_FILE)
//         {
//             cout << t.index << ":" << typeToStr(t.type)
//                  << ":\"" << t.value << "\"" << endl;
//         }
//     }
// }

// string Lexer::typeToStr(TokenType t)
// {
//     static const std::map<TokenType, string> names = {
//         {INT, "'int'"}, {VOID, "'void'"}, {IF, "'if'"}, {ELSE, "'else'"}, {WHILE, "'while'"}, {BREAK, "'break'"}, {CONTINUE, "'continue'"}, {RETURN, "'return'"}, {IDENTIFIER, "Ident"}, {INTCONST, "IntConst"}, {PLUS, "'+'"}, {MINUS, "'-'"}, {MULTIPLY, "'*'"}, {DIVIDE, "'/'"}, {MODULO, "'%'"}, {ASSIGN, "'='"}, {EQUAL, "'=='"}, {NOT_EQUAL, "'!='"}, {LESS, "'<'"}, {LESS_EQUAL, "'<='"}, {GREATER, "'>'"}, {GREATER_EQUAL, "'>='"}, {AND, "'&&'"}, {OR, "'||'"}, {NOT, "'!'"}, {LEFT_PAREN, "'('"}, {RIGHT_PAREN, "')'"}, {LEFT_BRACE, "'{'"}, {RIGHT_BRACE, "'}'"}, {SEMICOLON, "';'"}, {COMMA, "','"}, {END_OF_FILE, "EOF"}};

//     auto it = names.find(t);
//     return (it != names.end()) ? it->second : "UNKNOWN";
// }

// Token Lexer::createToken(TokenType type, const string &val, int startLine)
// {
//     return Token(type, val, tokenIndex++, startLine);
// }
