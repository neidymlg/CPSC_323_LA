#include <string>
#include <iostream>
#include <unordered_map>
#include <vector>
#include <cctype> //for tolower
#include <iomanip>

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
    Token lexer(FILE* filePointer) {
        string token;
        while (!feof(filePointer)) {
            myChar = getc(filePointer);

            if (myChar == EOF) {
                return Token(TokenType::UNKNOWN, ""); // Return empty token to signal EOF
            }

            switch (state) {
            case State::START:
                if (isdigit(myChar) || myChar == '.') {
                    return FSM_int_real(myChar, filePointer);
                }
                else if (isalpha(myChar)) {
                    return FSM_identifier(filePointer);
                }
                else if (isOperator(myChar)) {
                    char nextChar = getc(filePointer);
                    string op(1, myChar);
                    if ((myChar == '<' || myChar == '>' || myChar == '=' || myChar == '!') && nextChar == '=') {
                        op += nextChar;  // <=, >=, ==, !=
                    }
                    else {
                        ungetc(nextChar, filePointer);
                    }
                    return Token(TokenType::OPERATOR, op);
                }
                else if (isSeparator(myChar)) {
                    return Token(TokenType::SEPARATOR, string(1, myChar));
                }
                else if (myChar == '[') {
                    state = State::COMMENT;
                }
                else if (myChar == ' ' || myChar == '\n' || myChar == '\t') {
                    continue;
                }
                else {
                    return Token(TokenType::UNKNOWN, string(1, myChar));
                }
                break;

            case State::COMMENT:
                if (myChar == ']') {
                    state = State::START;
                }
                break;
            }
        }
        return Token(TokenType::UNKNOWN, "");
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

    Token FSM_identifier(FILE* filePointer) {
        string lexeme;
        int state = 0;
    
        while (!feof(filePointer)) {
            switch (state) {
                case 0:
                    if (isalpha(myChar) || myChar == '_') {
                        lexeme += myChar;
                        state = 1;
                    } else {
                        return Token(TokenType::UNKNOWN, string(1, myChar));
                    }
                    break;
    
                case 1:
                    myChar = getc(filePointer);
                    if (isalnum(myChar) || myChar == '_') {
                        lexeme += myChar;
                    } else {
                        ungetc(myChar, filePointer);
                        if (keywords.find(lexeme) != keywords.end()) {
                            return Token(TokenType::KEYWORD, lexeme);
                        }
                        return Token(TokenType::IDENTIFIER, lexeme);
                    }
                    break;
            }
        }
        return Token(TokenType::IDENTIFIER, lexeme);
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

    Token FSM_int_real(char myChar, FILE* filePointer) {
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
        return tokens.back();
    }
};

int main() {
    string FILE_NAME = "LA_input_1.txt";
    FILE* filePointer = fopen(FILE_NAME.c_str(), "r");

    if (!filePointer) {
        std::cout << "Error: Cannot open file " << FILE_NAME << std::endl;
        return 1;
    }

    LexicalAnalyzer la;
    std::vector<Token> tokens;

    while (!feof(filePointer)) {
        Token token = la.lexer(filePointer);
        if (token.value.empty()) {
            break;
        }
        tokens.push_back(token);
    }

    fclose(filePointer);

    std::cout << "\nOutput:\n";
    std::cout << "                    token                             lexeme\n";
    std::cout << "------------------------------------------------------------\n";

    for (size_t i = 0; i < tokens.size(); ++i) {
        string type;
        switch (tokens[i].type) {
        case TokenType::KEYWORD: type = "keyword";
            break;
        case TokenType::IDENTIFIER: type = "identifier";
            break;
        case TokenType::INTEGER: type = "integer";
            break;
        case TokenType::REAL: type = "real";
            break;
        case TokenType::OPERATOR: type = "operator";
            break;
        case TokenType::SEPARATOR: type = "separator";
            break;
        default: type = "unknown";
            break;
        }

        std::cout << "                   " << std::left << std::setw(35) << type << tokens[i].value << std::endl;
    }

    return 0;
}
