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
            type = "INTEGER";
            break;
        case static_cast<int>(TokenType::REAL):
            type = "REAL";
            break;
        case static_cast<int>(TokenType::OPERATOR):
            type = "OPERATOR";
            break;
        case static_cast<int>(TokenType::SEPARATOR):
            type = "SEPERATOR";
            break;
        default:
            type = "UNKNOWN";
            break;
        }
        cout << "<" << type << ", " << token.value << ">" << endl;
    }
}

// most likely change
vector<Token> LexicalAnalyzer::tokensize(string input) {
    vector<Token> tokens;
    string token = "";
    int state = 0;
    
    for (int i = 0; i < input.size(); i++) {
        char c = input[i];

        if (isspace(c)) {
            if (!token.empty()) {
                analyze(token);
                token = "";
            }
            continue;
        }
        if (isLetter(c)) {
            state = 1;
            token += c;
            while (i + 1 < input.size() && isValidIdentifierChar(input[i + 1])) {
                token += input[++i];
            }
            analyze(token);
            token = "";
        } 
        else if (isDigit(c)) {
            state = 2;
            token += c;

            while (i + 1 < input.size() && isDigit(input[i + 1])) {
                token += input[++i];
            }

            if (i + 1 < input.size() && input[i + 1] == '.') {
                token += input[++i];
                state = 3;

                if (i + 1 < input.size() && isDigit(input[i + 1])) {
                    token += input[++i];
                    state = 4;
                    
                    while (i + 1 < input.size() && isDigit(input[i + 1])) {
                        token += input[++i];
                    }
                    analyze(token);
                } else {
                    analyze(token);
                }
            } else {
                analyze(token);
            }
            token = "";
        }
        else if (isOperator(c)) {
            token += c;
            analyze(token);
            token = "";
        } 
        else if (isPunctuator(c)) {
            token += c;
            analyze(token);
            token = "";
        }
        else {
            token += c;
            analyze(token);
            token = "";
        }
    }
    return tokens;
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

// identifier ; upper case and lower case are the same
bool LexicalAnalyzer::isValidIdentifierChar(char c) {

    return isLetter(c) || isDigit(c) || c == '_';
}

// integer 
bool LexicalAnalyzer::isValidIntegerLiteral(string str) {
    for (char c : str) {
        if (!isDigit(c)) {
            return false;
        }
    }
    return true;
}

// void LexicalAnalyzer::addToken(TokenType type, string value) {
//     tokens.push_back(Token(type, value));
// }


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


// real: is integer followed by “.” and Integer, e.g., 123.00

bool LexicalAnalyzer::isValidRealLiteral(string str) {
    bool hasDecimal = false;
    for (char c : str) {
        if (c == '.') {
            if (hasDecimal) {
                return false;
            }
            hasDecimal = true;
        }
        else if (!isDigit(c)) {
            return false;
        }
    }
    return hasDecimal;
}

//lexer

void LexicalAnalyzer::lexer(string filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening file!" << endl;
        return;
    }
    
    string line;
    while (getline(file, line)) {
        tokensize(line);
    }
    
    file.close();
}
