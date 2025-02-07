#include <string>
#include <iostream>
#include <unordered_map>
#include <vector>
#include <fstream>

using namespace std;

// Enum class to define different types of tokens
enum class TokenType {
    KEYWORD,
    IDENTIFIER,
    INTEGER,
    REAL,
    OPERATOR,
    SEPARATOR,
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

    void analyze(const string& token){
        if (isValidInteger(token)) {
            tokens.push_back(Token(TokenType::INTEGER, token));
        }
        else {
            tokens.push_back(Token(TokenType::UNKNOWN, token));
        }
    }

    vector<Token> getTokens() {
        return tokens;
    }

private:
    bool isDigit(char c) {
        return c >= '0' && c <= '9';
    }

    bool isValidInteger(string str) {
        for (char c : str) {
            if (!isDigit(c)) {
                return false;
            }
        }
        return true;
    }
};

int main(){

    LexicalAnalyzer la;
    ifstream inputFile("LA_input_1.txt");
    string input;   

    while(inputFile >> input){
        la.analyze(input);
    }
    
    vector<Token> tokens = la.getTokens();
    for (Token token : tokens) {
        cout << "Type: " << static_cast<int>(token.type) << ", Value: " << token.value << endl;
    }
    inputFile.close();
}