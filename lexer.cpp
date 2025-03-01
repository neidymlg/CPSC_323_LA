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
    
    int identifier[6][3] = {
        {2, 6, 6},
        {3, 4, 5},
        {3, 4, 5},
        {3, 4, 5},
        {3, 4, 5},
        {6, 6, 6}
    };

    int int_union_real[5][2] = {
        {2, 5},
        {2, 3},
        {4, 5},
        {4, 5},
        {5, 5}
    };

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
        keywords["return"] = TokenType::KEYWORD;
        keywords["break"] = TokenType::KEYWORD;
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

    Token lexer(FILE* filePointer) {

        while ((myChar = getc(filePointer)) != EOF) {
            switch (state) {
            case State::START:
                if (isdigit(myChar) || myChar == '.') {
                    return FSM_int_real(filePointer);
                }
                else if (isalpha(myChar)) {
                    return FSM_identifier(filePointer);
                }
                else if (isOperator(myChar)) {
                    char nextChar = getc(filePointer);
                    string op(1, myChar);
                    if (nextChar == '=') {
                        op += nextChar; // for operators like <=, >=, ==, !=
                    }
                    else {
                        ungetc(nextChar, filePointer);
                    }
                    return Token(TokenType::OPERATOR, op);
                }
                else if (isSeparator(myChar)) {
                    return Token(TokenType::SEPARATOR, string(1, myChar));
                }
                else if(myChar == '$'){
                    char nextChar = getc(filePointer);
                    if(myChar == '$' && nextChar == '$'){
                        return Token(TokenType::SEPARATOR, "$$");
                    }
                    else{
                        ungetc(nextChar, filePointer);
                        return Token(TokenType::UNKNOWN, "$");
                    }
                }
                else if (myChar == '[') {
                    state = State::COMMENT;
                }
                else if (isWhiteSpace(myChar)) {
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

    bool isWhiteSpace(char c){
        return c == ' ' || c == '\n' || c=='\t';
    }

    Token FSM_identifier(FILE* filePointer) {
        string lexeme;
    
        lexeme += tolower(myChar);
        int state = 2;

        while ((myChar = getc(filePointer)) != EOF) {
            if(isalpha(myChar)){
                myChar = tolower(myChar);
                lexeme += myChar;
                state = identifier[state-1][0];
            }
            else if(isdigit(myChar)){
                lexeme += myChar;
                state = identifier[state-1][1];
            }
            else if(myChar == '_'){
                lexeme += myChar;
                state = identifier[state-1][2];
            }
            else if(isWhiteSpace(myChar) || isOperator(myChar) || isSeparator(myChar)){
                ungetc(myChar, filePointer);
                if (keywords.find(lexeme) != keywords.end()) {
                    return Token(TokenType::KEYWORD, lexeme);
                }
                else if (state > 1 && state < 6){
                    return Token(TokenType::IDENTIFIER, lexeme);
                }
                else{
                    return Token(TokenType::UNKNOWN, lexeme);
                }
            }
            else {
                lexeme += myChar;
                state = 6;
             
            }
        }

        return Token(TokenType::IDENTIFIER, lexeme);
    }
    

    Token FSM_int_real(FILE* filePointer) {
        string token;
        int state = 1;

        token += myChar;

        if (myChar == '.') {
            state = int_union_real[state - 1][1];
        }
        else {
            state = int_union_real[state - 1][0];
        }

        while ((myChar = getc(filePointer)) != EOF) {
            if (isdigit(myChar)) {
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
                    return Token(TokenType::INTEGER, token);
                }
                else if (state == 4) {
                    return Token(TokenType::REAL, token);
                }
                else {
                    return Token(TokenType::UNKNOWN, token);
                }
            }
        }
        return Token(TokenType::INTEGER, token);
    }
};

int main() {
    string FILE_NAME;
    cout << "Please enter the file name (LA_input_1.txt , LA_input_2.txt, LA_input_3.txt): ";
    getline(cin, FILE_NAME);

    FILE* filePointer = fopen(FILE_NAME.c_str(), "r");

    if (!filePointer) {
        std::cout << "Error: Cannot open file " << FILE_NAME << std::endl;
        return 1;
    }

    LexicalAnalyzer la;
    std::vector<Token> tokens;

    while (true) {
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
