#include <string>
#include <iostream>
#include <unordered_map>
#include <vector>
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
    std::string value;

    Token(TokenType type, string value) : type(type), value(value) {}
};

class LexicalAnalyzer {
    unordered_map<string, TokenType> keywords;
    vector<Token> tokens;
public:
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

    Token lexer(FILE* filePointer) {
        char myChar = getc(filePointer);
        TokenType myTokenType;
        while(true) {
            if(isWhiteSpace(myChar)) {
                //change state
                //go forward one char
                myChar = getc(filePointer);
                break;
            } else if(isKeyword(myChar)){
                break;
            } else {
                //error
                break;
            }
        }
        
        
        
        std::string tokenString = std::string(1, myChar);
        return Token(TokenType::IDENTIFIER, tokenString);
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

    bool isWhiteSpace(char c) {
        //implement Regex FSM
        if(c == ' ') {
            return true;
        }
        return false;
    }

    bool isKeyword(char c) {
        //implement Regex FSM for keywords
        if(isalpha(c)) {
            return true;
        }
        return false;
    }

};

int main() {

    string FILE_NAME = "LA_input_1.txt";
    //open the file and store a pointer acting as a cursor.
    FILE *filePointer = fopen("LA_input_1.txt", "r");

    if(filePointer == nullptr) {
        std::cout << "Whoops! Looks like there was an error finding the specified file." << std::endl;
        return 1;
    } else {
        std::cout << "Opened File: " << FILE_NAME << std::endl;
    }
    
    LexicalAnalyzer la;
    //get each char from the file one by one.
    //  - getc(filePointer) to get the next char.
    //  - unget(1, filePointer) to move the cursor back one.
    while(true) {
        try {
            Token myToken = la.lexer(filePointer);
            std::cout << myToken.value << std::endl;
        } catch (std::string error) {
            std::cout << error << std::endl;
            break;
        }
        if(feof(filePointer)) {
            break;
        }

    }

    //close the file.
    auto fileCloseError = fclose(filePointer);
    if(!fileCloseError) {
        std::cout << "Closed File: " << FILE_NAME << std::endl;
    } else {
        std::cout << "Error closing file: " << FILE_NAME << std::endl;
        return 1;
    }
    return 0;
}