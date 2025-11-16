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

#include "lexer.h"
#include <cctype>
#include <map>
#include <algorithm>

Lexer::Lexer(string source)
    : m_source(std::move(source)),
      m_pos(0),
      m_tokenIdCounter(0),
      m_currentLine(1)
{
    m_keywords = {
        {"int", TokenType::INT}, {"void", TokenType::VOID}, {"if", TokenType::IF}, {"else", TokenType::ELSE}, {"while", TokenType::WHILE}, {"break", TokenType::BREAK}, {"continue", TokenType::CONTINUE}, {"return", TokenType::RETURN}};
}

bool Lexer::isAtEnd() const
{
    return m_pos >= m_source.length();
}

char Lexer::currentChar() const
{
    return isAtEnd() ? '\0' : m_source[m_pos];
}

char Lexer::peekNextChar() const
{
    return (m_pos + 1 >= m_source.length()) ? '\0' : m_source[m_pos + 1];
}

void Lexer::advance()
{
    if (!isAtEnd())
    {
        if (m_source[m_pos] == '\n')
            ++m_currentLine;
        ++m_pos;
    }
}

void Lexer::skipWhitespace()
{
    while (!isAtEnd() && std::isspace(static_cast<unsigned char>(currentChar())))
        advance();
}

void Lexer::skipComments()
{
    if (currentChar() != '/' || (peekNextChar() != '/' && peekNextChar() != '*'))
        return;

    bool isLineComment = (peekNextChar() == '/');
    advance();
    advance();

    if (isLineComment)
    {
        while (!isAtEnd() && currentChar() != '\n')
            advance();
    }
    else
    {
        while (!isAtEnd() && !(currentChar() == '*' && peekNextChar() == '/'))
            advance();
        if (!isAtEnd())
        {
            advance();
            advance();
        }
    }
}

Token Lexer::consumeNumber()
{
    const int startLine = m_currentLine;
    string numText;

    const char first = currentChar();
    numText += first;
    advance();

    if (first == '0')
    {
        if (!isAtEnd() && std::isdigit(static_cast<unsigned char>(currentChar())))
        {
            while (!isAtEnd() && std::isdigit(static_cast<unsigned char>(currentChar())))
            {
                numText += currentChar();
                advance();
            }
            return makeToken(TokenType::UNKNOWN, numText, startLine);
        }
        return makeToken(TokenType::INTCONST, numText, startLine);
    }

    while (!isAtEnd() && std::isdigit(static_cast<unsigned char>(currentChar())))
    {
        numText += currentChar();
        advance();
    }

    return makeToken(TokenType::INTCONST, numText, startLine);
}

Token Lexer::consumeIdentifier()
{
    const int startLine = m_currentLine;
    string idText;

    auto isIdChar = [](char c)
    { return std::isalnum(static_cast<unsigned char>(c)) || c == '_'; };

    while (!isAtEnd() && isIdChar(currentChar()))
    {
        idText += currentChar();
        advance();
    }

    auto it = m_keywords.find(idText);
    TokenType type = (it != m_keywords.end()) ? it->second : TokenType::IDENTIFIER;

    return makeToken(type, idText, startLine);
}

Token Lexer::consumeOperator()
{
    const int startLine = m_currentLine;
    const char c1 = currentChar();
    advance();

    static const std::unordered_map<string, TokenType> dblOps = {
        {"==", TokenType::EQUAL}, {"!=", TokenType::NOT_EQUAL}, {"<=", TokenType::LESS_EQUAL}, {">=", TokenType::GREATER_EQUAL}, {"&&", TokenType::AND}, {"||", TokenType::OR}};

    static const std::unordered_map<char, TokenType> sglOps = {
        {'+', TokenType::PLUS}, {'-', TokenType::MINUS}, {'*', TokenType::MULTIPLY}, {'/', TokenType::DIVIDE}, {'%', TokenType::MODULO}, {'=', TokenType::ASSIGN}, {'!', TokenType::NOT}, {'<', TokenType::LESS}, {'>', TokenType::GREATER}, {'(', TokenType::LEFT_PAREN}, {')', TokenType::RIGHT_PAREN}, {'{', TokenType::LEFT_BRACE}, {'}', TokenType::RIGHT_BRACE}, {';', TokenType::SEMICOLON}, {',', TokenType::COMMA}};

    if (!isAtEnd())
    {
        string dblChar{c1, currentChar()};
        auto it = dblOps.find(dblChar);
        if (it != dblOps.end())
        {
            advance();
            return makeToken(it->second, dblChar, startLine);
        }
    }

    auto it = sglOps.find(c1);
    TokenType type = (it != sglOps.end()) ? it->second : TokenType::UNKNOWN;

    return makeToken(type, string(1, c1), startLine);
}

Token Lexer::nextToken()
{
    while (!isAtEnd())
    {
        skipWhitespace();
        if (isAtEnd())
            break;

        const char c = currentChar();
        const char n = peekNextChar();

        if (c == '/' && (n == '/' || n == '*'))
        {
            skipComments();
            continue;
        }

        if (std::isdigit(static_cast<unsigned char>(c)))
            return consumeNumber();

        if (std::isalpha(static_cast<unsigned char>(c)) || c == '_')
            return consumeIdentifier();

        return consumeOperator();
    }

    return makeToken(TokenType::END_OF_FILE, "", m_currentLine);
}

vector<Token> Lexer::getAllTokens()
{
    vector<Token> tokens;
    Token token;
    do
    {
        token = nextToken();
        tokens.push_back(token);
    } while (token.type != TokenType::END_OF_FILE);
    return tokens;
}

void Lexer::output()
{
    auto tokens = getAllTokens();
    for (const auto &t : tokens)
    {
        if (t.type != TokenType::END_OF_FILE)
        {
            cout << t.index << ":" << typeToStr(t.type)
                 << ":\"" << t.value << "\"" << endl;
        }
    }
}

string Lexer::typeToStr(TokenType t) const
{
    static const std::map<TokenType, string> names = {
        {TokenType::INT, "'int'"}, {TokenType::VOID, "'void'"}, {TokenType::IF, "'if'"}, {TokenType::ELSE, "'else'"}, {TokenType::WHILE, "'while'"}, {TokenType::BREAK, "'break'"}, {TokenType::CONTINUE, "'continue'"}, {TokenType::RETURN, "'return'"}, {TokenType::IDENTIFIER, "Ident"}, {TokenType::INTCONST, "IntConst"}, {TokenType::PLUS, "'+'"}, {TokenType::MINUS, "'-'"}, {TokenType::MULTIPLY, "'*'"}, {TokenType::DIVIDE, "'/'"}, {TokenType::MODULO, "'%'"}, {TokenType::ASSIGN, "'='"}, {TokenType::EQUAL, "'=='"}, {TokenType::NOT_EQUAL, "'!='"}, {TokenType::LESS, "'<'"}, {TokenType::LESS_EQUAL, "'<='"}, {TokenType::GREATER, "'>'"}, {TokenType::GREATER_EQUAL, "'>='"}, {TokenType::AND, "'&&'"}, {TokenType::OR, "'||'"}, {TokenType::NOT, "'!'"}, {TokenType::LEFT_PAREN, "'('"}, {TokenType::RIGHT_PAREN, "')'"}, {TokenType::LEFT_BRACE, "'{'"}, {TokenType::RIGHT_BRACE, "'}'"}, {TokenType::SEMICOLON, "';'"}, {TokenType::COMMA, "','"}, {TokenType::END_OF_FILE, "EOF"}};
    auto it = names.find(t);
    return (it != names.end()) ? it->second : "UNKNOWN";
}

Token Lexer::makeToken(TokenType type, const string &text, int startLine)
{
    return Token{type, text, m_tokenIdCounter++, startLine};
}