#include <string>
#include <iostream>
#include <unordered_map>
#include <vector>
#include <fstream>

using namespace std;

// Enum class to define different types of tokens
enum class TokenType {
    KEYWORD, //complete
    IDENTIFIER, //complete
    INTEGER, //complete
    REAL,
    OPERATOR, //complete
    SEPARATOR, //complete
    UNKNOWN //complete
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

        if(keywords.find(token) != keywords.end()){
            tokens.push_back(Token(TokenType::KEYWORD, token));
        }
        else if (isValidInteger(token)) {
            tokens.push_back(Token(TokenType::INTEGER, token));
        }
        else if (isOperator(token[0])) {
            tokens.push_back(Token(TokenType::OPERATOR, token));
        }
        else if (isSeparator(token[0])) {
            tokens.push_back(Token(TokenType::SEPARATOR, token));
        }
        else if (isValidIdentifier(token)) {
            tokens.push_back(Token(TokenType::IDENTIFIER, token));
        }
        else {
            tokens.push_back(Token(TokenType::UNKNOWN, token));
        }
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
            case TokenType::INTEGER:
                type = "INTEGER";
                break;
            case TokenType::REAL:
                type = "REAL";
                break;
            case TokenType::OPERATOR:
                type = "OPERATOR";
                break;
            case TokenType::SEPARATOR:
                type = "PUNCTUATOR";
                break;
            default:
                type = "UNKNOWN";
                break;
            }
            cout << "<" << type << ", " << token.value << ">" << endl;
        }
    }

private:
    bool isOperator(char c) {
        return c == '+' || c == '-' || c == '*' || c == '/' || c == '%' || c == '=' || c == '<' || c == '>' || c == '!';
    }

    bool isSeparator(char c) {
        return c == ';' || c == ',' || c == '(' || c == ')' || c == '{' || c == '}';
    }

    bool isDigit(char c) {
        return c >= '0' && c <= '9';
    }

    bool isLetter(char c) {
        return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
    }
    bool isValidIdentifierChar(char c){
        return isLetter(c) || isDigit(c) || c == '_';
    }

    bool isValidInteger(string str) {
        for (char c : str) {
            if (!isDigit(c)) {
                return false;
            }
        }
        return true;
    }

    bool isValidIdentifier(string str){
        if(!isLetter(str[0])){
            return false;
        }

        for (char c : str) {
            if (!isValidIdentifierChar(c)) {
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
    
    inputFile.close();
    
    la.printTokens();

}