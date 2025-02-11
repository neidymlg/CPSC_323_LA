#ifndef LA_H
#define LA_H

#include <string>
#include <iostream>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <cctype>
#include <sstream>

using namespace std;

enum class TokenType {
    KEYWORD,
    IDENTIFIER,
    INTEGER,
    OPERATOR,
    PUNCTUATOR,
    SEQUENCE,
    UNKNOWN
};

struct Token {
    TokenType type;
    string value;

    Token(TokenType type, string value);
};

class LexicalAnalyzer {
    unordered_map<string, TokenType> keywords;
    vector<Token> tokens;

public:
    LexicalAnalyzer();
    vector<Token> analyze(string input);
    void printTokens();
    vector<Token> tokenize(string input);

private:
    bool isOperator(char c);
    bool isPunctuator(char c);
    bool isDigit(char c);
    bool isLetter(char c);
    bool isValidIdentifierChar(char c);
    bool isValidIntegerLiteral(string str);
    bool isCommentStart(char current, char next);
    void handleComment(string input, int& pos);
    string handleStringLiteral(string input, int& pos);
    string getNextToken(string input, int& pos);
    void addToken(TokenType type, string value);
};

#endif // LA_H
