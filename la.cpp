#include "la.h"


Token::Token(TokenType type, string value) : type(type), value(value) {}


LexicalAnalyzer::LexicalAnalyzer() {
    keywords["integer"] = TokenType::KEYWORD;
    keywords["real"] = TokenType::KEYWORD;
    keywords["if"] = TokenType::KEYWORD;
    keywords["else"] = TokenType::KEYWORD;
    keywords["endif"] = TokenType::KEYWORD;
    keywords["while"] = TokenType::KEYWORD;
    keywords["endwhile"] = TokenType::KEYWORD;
    keywords["scan"] = TokenType::KEYWORD;
    keywords["print"] = TokenType::KEYWORD;
    keywords["function"] = TokenType::KEYWORD;
    keywords["boolean"] = TokenType::KEYWORD;
    keywords["true"] = TokenType::KEYWORD;
    keywords["false"] = TokenType::KEYWORD;
}

void LexicalAnalyzer::analyze(const string& token) {
    if (keywords.find(token) != keywords.end()) {
        tokens.push_back(Token(TokenType::KEYWORD, token));
    } else if (isValidIntegerLiteral(token)) {
        tokens.push_back(Token(TokenType::INTEGER, token));
    } else if (isOperator(token[0])) {
        tokens.push_back(Token(TokenType::OPERATOR, token));
    } else if (isPunctuator(token[0])) {
        tokens.push_back(Token(TokenType::SEPARATOR, token));
    } else {
        tokens.push_back(Token(TokenType::UNKNOWN, token));
    }

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
        case static_cast<int>(TokenType::SEPARATOR):
            type = "PUNCTUATOR";
            break;
        default:
            type = "UNKNOWN";
            break;
        }
        cout << "<" << type << ", " << token.value << ">" << endl;
    }
}

// vector<Token> LexicalAnalyzer::tokenize(string input) {
//     return analyze(input);
// }

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

// string LexicalAnalyzer::getNextToken(string input, int& pos) {
//     string token = "";
//     if (pos == input.size()) {
//         return token;
//     }
//     if (isOperator(input[pos])) {
//         token += input[pos++];
//     }
//     else if (isPunctuator(input[pos])) {
//         token += input[pos++];
//     }
//     else if (isDigit(input[pos])) {
//         while (pos < input.size() && isDigit(input[pos])) {
//             token += input[pos++];
//         }
//     }
//     else if (isLetter(input[pos])) {
//         while (pos < input.size() && isValidIdentifierChar(input[pos])) {
//             token += input[pos++];
//         }
//     }
//     else {
//         token += input[pos++];
//     }
//     return token;
// }
