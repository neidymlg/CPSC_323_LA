#include <string>
#include <iostream>
#include <unordered_map>
#include <vector>
#include <cctype> //for tolower

using namespace std;

enum class TokenType {
    KEYWORD, 
    IDENTIFIER, 
    INTEGER, 
    REAL,
    OPERATOR, 
    SEPARATOR,
    UNKNOWN
};

enum State {
    START,
    IDENTIFIER,
    COMMENT,
    INTEGER,
    CHECKREAL,
    REAL,
    OPERATOR,
    SEPARATOR
};

struct Token {
    TokenType type;
    std::string value;

    Token(TokenType type, string value) : type(type), value(value) {}
};

class LexicalAnalyzer {
    unordered_map<string, TokenType> keywords;
    vector<Token> tokens;
    char myChar;
    State state = State::START;

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

    void lexer(FILE* filePointer) {
        string token;

        while(!feof(filePointer)) {
            myChar = getc(filePointer);

            switch(state) {
                case State::START:
                    token = myChar;
                    if(isDigit(myChar)){
                        state = State::INTEGER;
                    }
                    else if(isLetter(myChar)) {
                        token = tolower(myChar);
                        state = State::IDENTIFIER;
                    }
                    break;
                case State::IDENTIFIER:
                    if(isIdentifier(myChar)) {
                        token += tolower(myChar);
                    } else {
                        ungetc(myChar, filePointer);
                        if(keywords.find(token) != keywords.end()) {
                            tokens.push_back(Token(TokenType::KEYWORD, token));
                        } else {
                            tokens.push_back(Token(TokenType::IDENTIFIER, token));
                        }
                        state = State::START;
                    }
                case State::COMMENT:
                    break;
                case State::INTEGER:
                    if(isDigit(myChar)) {
                        token += myChar;
                    } else if(myChar == '.') {
                        token += myChar;
                        state = State::CHECKREAL;
                    } else {
                        ungetc(myChar, filePointer);
                        tokens.push_back(Token(TokenType::INTEGER, token));
                        state = State::START;
                    }
                    break;
                case State::CHECKREAL:
                    if(isDigit(myChar)) {
                        token += myChar;
                        state = State::REAL;
                    } else {
                        ungetc(myChar, filePointer);
                        tokens.push_back(Token(TokenType::UNKNOWN, token));
                        state = State::START;
                    }
                    break;
                case State::REAL:
                    if(isDigit(myChar)) {
                        token += myChar;
                    } else {
                        ungetc(myChar, filePointer);
                        tokens.push_back(Token(TokenType::REAL, token));
                        state = State::START;
                    }
                    break;
                case State::OPERATOR:
                    break;
                case State::SEPARATOR:
                    break;
                default:
                    break;
            }
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
        return (c >= 'a' && c <= 'z') | (c >= 'A' && c <= 'Z');
    }

    bool isIdentifier(char c) {
        //implement Regex FSM for identifiers
        if((c >= 'a' && c <= 'z') || isDigit(c) || c == '_') {
            return true;
        }
        return false;
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
    la.lexer(filePointer);
    // while(true) {
    //     try {
    //         la.lexer(filePointer);
    //     } catch (std::string error) {
    //         std::cout << error << std::endl;
    //         break;
    //     }
    //     if(feof(filePointer)) {
    //         break;
    //     }

    // }

    //close the file.
    auto fileCloseError = fclose(filePointer);
    if(!fileCloseError) {
        std::cout << "Closed File: " << FILE_NAME << std::endl;
    } else {
        std::cout << "Error closing file: " << FILE_NAME << std::endl;
        return 1;
    }

    la.printTokens();
    return 0;
}