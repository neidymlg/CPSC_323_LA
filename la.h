#pragma once

#include <string>
#include <iostream>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <cctype>
#include <sstream>
#include <fstream>


using namespace std;

enum class TokenType {
    KEYWORD, //complete
    IDENTIFIER, //complete
    INTEGER, //complete
    REAL,
    OPERATOR, //complete
    SEPARATOR, //complete
    UNKNOWN //complete
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
    void setInput(string input);
    void analyze(const string& token);
    string getNextToken(string input, int& pos);
    bool hasMoreTokens(string input, int pos);
    void printTokens();
    vector<Token> tokensize(string input);
    void lexer(string filename);

private:
    bool isOperator(char c);
    bool isPunctuator(char c);
    bool isDigit(char c);
    bool isLetter(char c);
    bool isValidIdentifierChar(char c);
    bool isValidIntegerLiteral(string str);
    bool isValidRealLiteral(string str);
    void addToken(TokenType type, string value);
    string extractNextToken(string input);
};
