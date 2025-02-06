#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
using namespace std;

// Enum class to define different types of tokens
enum class TokenType {
    KEYWORD,
    IDENTIFIER,
    INTEGER_LITERAL,
    FLOAT_LITERAL,
    OPERATOR,
    PUNCTUATOR,
    UNKNOWN
};

// Struct to represent a token with its type and value
struct Token { 
    TokenType type;
    string value;

    Token(TokenType type, string value) : type(type), value(value) {}
};

class LexicalAnalyzer { 
    unordered_map<string, TokenType> keywords;
    vector<Token> tokens;

public:
    // Constructor to initialize the keywords
    LexicalAnalyzer() {
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

    vector<Token> analyze(string input) {
        int pos = 0;
        while (pos < input.size()) {
            string token = getNextToken(input, pos);
            if (token.empty()) {
                break;
            }
            if (keywords.find(token) != keywords.end()) {
                addToken(TokenType::KEYWORD, token);
            } else if (isValidIntegerLiteral(token)) {
                addToken(TokenType::INTEGER_LITERAL, token);
            } else if (isValidFloatLiteral(token)) {
                addToken(TokenType::FLOAT_LITERAL, token);
            } else if (isOperator(token[0])) {
                addToken(TokenType::OPERATOR, token);
            } else if (isPunctuator(token[0])) {
                addToken(TokenType::PUNCTUATOR, token);
            } else {
                addToken(TokenType::IDENTIFIER, token);
            }
        }
        return tokens;
    }

    // Function to print the list of tokens
    void printTokens() {
        for (Token token : tokens) {
            string type;
            switch (token.type) {
                case TokenType::KEYWORD:
                    type = "KEYWORD";
                    break;
                case TokenType::IDENTIFIER:
                    type = "IDENTIFIER";
                    break;
                case TokenType::INTEGER_LITERAL:
                    type = "INTEGER_LITERAL";
                    break;
                case TokenType::FLOAT_LITERAL:
                    type = "FLOAT_LITERAL";
                    break;
                case TokenType::OPERATOR:
                    type = "OPERATOR";
                    break;
                case TokenType::PUNCTUATOR:
                    type = "PUNCTUATOR";
                    break;
                default:
                    type = "UNKNOWN";
                    break;
            }
            cout << "<" << type << ", " << token.value << ">" << endl;
        }
    }
    // Function to tokenize the input string -- maybe delete
    vector<Token> tokenize(string input) {
        return analyze(input);
    }



    private:
    // Function to check if a character is an operator
    bool isOperator(char c) {
        return c == '+' || c == '-' || c == '*' || c == '/' || c == '%' || c == '=' || c == '<' || c == '>' || c == '!';
    }
    // Function to check if a character is a whitespace character
    bool isWhiteSpace(char c) {
        return c == ' ' || c == '\t' || c == '\n';
    }

    // check if the character is a punctuator
    bool isPunctuator(char c) {
        return c == ';' || c == ',' || c == '(' || c == ')' || c == '{' || c == '}';
    }

    // Function to check if a character is a digit
    bool isDigit(char c) {
        return c >= '0' && c <= '9';
    }

    // Function to check if a character is a letter
    bool isLetter(char c) {
        return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
    }

    // Function to check if a character is a valid identifier character
    bool isValidIdentifierChar(char c) {
        return isLetter(c) || isDigit(c) || c == '_';
    }

    // Function to check if a string is a valid integer literal
    bool isValidIntegerLiteral(string str) {
        for (char c : str) {
            if (!isDigit(c)) {
                return false;
            }
        }
        return true;
    }

    // Function to check if a string is a valid float literal
    bool isValidFloatLiteral(string str) {
        bool pointSeen = false;
        for (char c : str) {
            if (c == '.') {
                if (pointSeen) {
                    return false;
                }
                pointSeen = true;
            } else if (!isDigit(c)) {
                return false;
            }
        }
        return true;
    }

    // Function to add a token to the list of tokens
    void addToken(TokenType type, string value) {
        tokens.push_back(Token(type, value));
    }
    // Function to get the next token from the input string
    string getNextToken(string input, int &pos) {
        string token = "";
        while (pos < input.size() && isWhiteSpace(input[pos])) {
            pos++;
        }
        if (pos == input.size()) {
            return token;
        }
        if (isOperator(input[pos])) {
            token += input[pos++];
            while (pos < input.size() && isOperator(input[pos])) {
                token += input[pos++];
            }
        } else if (isPunctuator(input[pos])) {
            token += input[pos++];
        } else if (isDigit(input[pos])) {
            while (pos < input.size() && isDigit(input[pos])) {
                token += input[pos++];
            }
            if (pos < input.size() && input[pos] == '.') {
                token += input[pos++];
                while (pos < input.size() && isDigit(input[pos])) {
                    token += input[pos++];
                }
            }
        } else if (isLetter(input[pos])) {
            while (pos < input.size() && isValidIdentifierChar(input[pos])) {
                token += input[pos++];
            }
        } else {
            token += input[pos++];
        }
        return token;
    }
    

};