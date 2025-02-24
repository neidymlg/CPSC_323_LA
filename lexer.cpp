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
    COMMENT
};

// states
enum class LexerState {
    START,
    KEYWORD,
    IDENTIFIER,
    INTEGER,
    REAL,
    OPERATOR,
    SEPARATOR,
    UNKNOWN
};

int fsm_identifier[5][3] = {
    {2, 4, 5},
    {2, 3, 5},
    {2, 3, 5},
    {3, 3, 5},
    {5, 5, 5}
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
                type = "SEPARATOR";
                break;
            default:
                type = "UNKNOWN";
                break;
            }
            cout << "<" << type << ", " << token.value << ">" << endl;
        }
    }


    // Token lexer (FILE* filePointer) {
    //     string token;
    //     char myChar = getc(filePointer); and so forth
    void lexer(FILE* filePointer) {
        string token;

        while (!feof(filePointer)) {
            myChar = getc(filePointer);

            switch (state) {
            case State::START:
                if (isDigit(myChar) || myChar == '.') {
                    FSM_int_real(myChar, filePointer);
                }
                else if (isLetter(myChar)) {
                    ungetc(myChar, filePointer);

                    FSM_identifier(filePointer);
                }
                else if (isOperator(myChar)) {
                    //check if next character is a valid operator with two symbols...
                    if(myChar == '>' || myChar == '<' || myChar == '=') {
                        char secondChar = getc(filePointer);
                        std::cout << "grabbing second char after: " << myChar << std::endl;
                        std::cout << "second char: " << secondChar << std::endl;

                        if(myChar == '>' && secondChar == '=') {
                            token = myChar;
                            token += secondChar;
                        } else if (myChar == '<' && secondChar == '=') {
                            std::cout << "appending two characters." << std::endl;
                            token = myChar;
                            token += secondChar;
                            std::cout << "string to return: " << token << std::endl;
                        } else if (myChar == '=' && secondChar == '=') {
                            token = myChar;
                            token += secondChar;
                        } else {
                            //if not a valid 2 symbol operator, move filePtr back 1.
                            std::cout << "move back 1 char" << std::endl;
                            ungetc(secondChar, filePointer);
                        }
                    } else {
                        //this is a single symbol operator
                        token = myChar;
                    }
                    tokens.push_back(Token(TokenType::OPERATOR, token));
                }
                else if (isSeparator(myChar)) {
                    token = myChar;
                    tokens.push_back(Token(TokenType::SEPARATOR, token));
                }
                else if (myChar == '[') {
                    state = State::COMMENT;
                }
                break;
            case State::COMMENT:
                if (myChar == ']') {
                    state = State::START;
                }
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

    void FSM_identifier(FILE* filePointer) {
        string token;
        int state = 1;

        char myChar = getc(filePointer);
        token += myChar;

        if (isLetter(myChar) || myChar == '_') {
            state = fsm_identifier[state - 1][0];
        } else if (isDigit(myChar)) {
            state = fsm_identifier[state - 1][1];
        } else {
            state = fsm_identifier[state - 1][2];
        }

        while (!feof(filePointer)) {
            myChar = getc(filePointer);

            if (isLetter(myChar) || myChar == '_') {
                token += myChar;
                state = fsm_identifier[state - 1][0];
            } else if (isDigit(myChar)) {
                token += myChar;
                state = fsm_identifier[state - 1][1];
            } else {
                ungetc(myChar, filePointer);
                if (state == 2 || state == 3) {
                    if (keywords.find(token) != keywords.end()) {
                        tokens.push_back(Token(keywords[token], token));
                    } else {
                        tokens.push_back(Token(TokenType::IDENTIFIER, token));
                    }
                } else {
                    tokens.push_back(Token(TokenType::UNKNOWN, token));
                }
                break;
            }
        }
    }


    // all white spaces should be ignored
    // bool isWhiteSpace(char c) {
    //     //implement Regex FSM
    //     if(c == ' ') {
    //         return true;
    //     }
    //     return false;
    // }

    bool isKeyword(char c) {
        //implement Regex FSM for keywords
        if (isalpha(c)) {
            return true;
        }
        return false;
    }

    void FSM_int_real(char myChar, FILE* filePointer) {
        string token;
        int state = 1;

        int int_union_real[5][2] = {
            {2, 5},
            {2, 3},
            {4, 5},
            {4, 5},
            {5, 5}
        };

        token += myChar;

        if (myChar == '.') {
            state = int_union_real[state - 1][1];
        }
        else {
            state = int_union_real[state - 1][0];
        }

        while (!feof(filePointer)) {
            myChar = getc(filePointer);

            if (isDigit(myChar)) {
                token += myChar;
                state = int_union_real[state - 1][0];
            }
            else if (myChar == '.') {
                token += myChar;
                state = int_union_real[state - 1][1];
            }
            else {
                ungetc(myChar, filePointer);
                if (state == 2) {
                    tokens.push_back(Token(TokenType::INTEGER, token));
                }
                else if (state == 4) {
                    tokens.push_back(Token(TokenType::REAL, token));
                }
                else {
                    tokens.push_back(Token(TokenType::UNKNOWN, token));
                }
                break;
            }
        }
    }
};

int main() {

    string FILE_NAME = "LA_input_1.txt";
    //open the file and store a pointer acting as a cursor.
    FILE* filePointer = fopen("LA_input_1.txt", "r");

    if (filePointer == nullptr) {
        std::cout << "Whoops! Looks like there was an error finding the specified file." << std::endl;
        return 1;
    }
    else {
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
    if (!fileCloseError) {
        std::cout << "Closed File: " << FILE_NAME << std::endl;
    }
    else {
        std::cout << "Error closing file: " << FILE_NAME << std::endl;
        return 1;
    }
    std::cout << "\noutput:\n" << "token       lexeme" <<"\n----------------------" << std::endl;

    la.printTokens();
    return 0;
}