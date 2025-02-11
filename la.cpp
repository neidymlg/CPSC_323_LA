#include "la.h"


Token::Token(TokenType type, string value) : type(type), value(value) {}


LexicalAnalyzer::LexicalAnalyzer() {
    keywords["int"] = TokenType::KEYWORD;
    keywords["float"] = TokenType::KEYWORD;
    keywords["char"] = TokenType::KEYWORD;
    keywords["if"] = TokenType::KEYWORD;
    keywords["else"] = TokenType::KEYWORD;
    keywords["while"] = TokenType::KEYWORD;
    keywords["do"] = TokenType::KEYWORD;
    keywords["for"] = TokenType::KEYWORD;
    keywords["return"] = TokenType::KEYWORD;
}

vector<Token> LexicalAnalyzer::analyze(string input) {
    int pos = 0;
    while (pos < input.size()) {
        string token = getNextToken(input, pos);
        if (token.empty()) {
            break;
        }
        if (keywords.find(token) != keywords.end()) {
            addToken(TokenType::KEYWORD, token);
        }
        else if (isValidIntegerLiteral(token)) {
            addToken(TokenType::INTEGER, token);
        }
        else if (isOperator(token[0])) {
            addToken(TokenType::OPERATOR, token);
        }
        else if (isPunctuator(token[0])) {
            addToken(TokenType::PUNCTUATOR, token);
        }
        else {
            addToken(TokenType::IDENTIFIER, token);
        }
    }
    return tokens;
}

void LexicalAnalyzer::printTokens() {
    for (Token token : tokens) {
        string type;
        switch (static_cast<int>(token.type)) {
        case static_cast<int>(TokenType::KEYWORD):
            type = "KEYWORD";
            break;
        case static_cast<int>(TokenType::IDENTIFIER):
            type = "IDENTIFIER";
            break;
        case static_cast<int>(TokenType::INTEGER):
            type = "INTEGER_LITERAL";
            break;
        case static_cast<int>(TokenType::OPERATOR):
            type = "OPERATOR";
            break;
        case static_cast<int>(TokenType::PUNCTUATOR):
            type = "PUNCTUATOR";
            break;
        default:
            type = "UNKNOWN";
            break;
        }
        cout << "<" << type << ", " << token.value << ">" << endl;
    }
}

vector<Token> LexicalAnalyzer::tokenize(string input) {
    return analyze(input);
}

bool LexicalAnalyzer::isOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '%' || c == '=' || c == '<' || c == '>' || c == '!';
}

bool LexicalAnalyzer::isPunctuator(char c) {
    return c == ';' || c == ',' || c == '(' || c == ')' || c == '{' || c == '}';
}

bool LexicalAnalyzer::isDigit(char c) {
    return c >= '0' && c <= '9';
}

bool LexicalAnalyzer::isLetter(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

bool LexicalAnalyzer::isValidIdentifierChar(char c) {
    return isLetter(c) || isDigit(c) || c == '_';
}

bool LexicalAnalyzer::isValidIntegerLiteral(string str) {
    for (char c : str) {
        if (!isDigit(c)) {
            return false;
        }
    }
    return true;
}

void LexicalAnalyzer::addToken(TokenType type, string value) {
    tokens.push_back(Token(type, value));
}

string LexicalAnalyzer::getNextToken(string input, int& pos) {
    string token = "";
    if (pos == input.size()) {
        return token;
    }
    if (isOperator(input[pos])) {
        token += input[pos++];
    }
    else if (isPunctuator(input[pos])) {
        token += input[pos++];
    }
    else if (isDigit(input[pos])) {
        while (pos < input.size() && isDigit(input[pos])) {
            token += input[pos++];
        }
    }
    else if (isLetter(input[pos])) {
        while (pos < input.size() && isValidIdentifierChar(input[pos])) {
            token += input[pos++];
        }
    }
    else {
        token += input[pos++];
    }
    return token;
}
