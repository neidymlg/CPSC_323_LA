#include <string>
#include <iostream>
#include <unordered_map>
#include <vector>
#include <cctype> //for tolower
#include <iomanip>
#include <fstream> //for output files

using namespace std;

//token types, used for categorizing 
enum class TokenType {
    KEYWORD,
    IDENTIFIER,
    INTEGER,
    REAL,
    OPERATOR,
    SEPARATOR,
    UNKNOWN
};

//state types, used to skip comments
enum State {
    START,
    COMMENT
};

//Structure used to store token types and tokens, will be returned to the main function
struct Token {
    TokenType type;
    std::string value;
    Token(TokenType type, string value) : type(type), value(value) {}
};

class LexicalAnalyzer {
    unordered_map<string, TokenType> keywords; //to store keywords
    char myChar; //to get current character
    State state = State::START;
    
    //fsm for identifiers, int, and real
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
    //initalizes all keyword
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

    //reads all tokens, sends to the FSM functions to be categorized (return Token types)
    //or categorizes (returns token types) themselves
    Token lexer(FILE* filePointer) {

        //gets first chatacter
        // 1. if there is a number or punctuation, sends it so it can be categorized into int/real
        // 2. if there is a letter, sends it so it can be categorized into an identifier
        // 3. if there is an operator, checks if it is a double operator (<=, >=, etc) or not and categorizes it 
        // 4. if there is a seperator, sends it so it can be categorized into an seperator
        // 5. if there are two $, sends it so it can be categorized into an seperator
        // 6. if there is a [, changes it's state to comment so we can skip it
        // 7. if there is a whitespace, continues without categorizing (skips)
        // 8. if it is none of the above, categorizes as unknown

        //inside comment state, checks if we are at ] so it can go back to start state
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
        //in case there is an error, outputs nothing
        return Token(TokenType::UNKNOWN, "");
    }

private:

    //checks operator
    bool isOperator(char c) {
        return c == '+' || c == '-' || c == '*' || c == '/' || c == '%' || c == '=' || c == '<' || c == '>' || c == '!';
    }

    //checks seperator
    bool isSeparator(char c) {
        return c == ';' || c == ',' || c == '(' || c == ')' || c == '{' || c == '}';
    }

    //checks whitespace
    bool isWhiteSpace(char c){
        return c == ' ' || c == '\n' || c=='\t';
    }

    //categorizes fsm identifier
    Token FSM_identifier(FILE* filePointer) {
        string lexeme;
    
        //gets first string and starts as state 2 (since we went from 1 -> letter -> 2)
        lexeme += tolower(myChar);
        int state = 2;

        //gets next character
        //1. if letter, adds into lexeme, changes state 
        //2. if digit, adds into lexeme, changes state 
        //3. if _, adds into lexeme, changes state 
        //4. if whitespace/operator/seperator, puts char back to buffer, checks if it is a keyword
        //4. , identifier, or invalid combination and categorizes
        //5. if it is an invalid symbol, adds into lexeme, changes state to 6 to symbolize it is invalid
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

        //if there is only one letter, returns it as an identifier
        return Token(TokenType::IDENTIFIER, lexeme);
    }
    

    //categorizes fsm integer or real
    Token FSM_int_real(FILE* filePointer) {
        string token;
        int state = 1;

        //adds first character into token
        token += myChar;

        //1. if first character is ., changes state (this is an invalid state)
        //2. if first character is digit, changes state
        if (myChar == '.') {
            state = int_union_real[state - 1][1];
        }
        else {
            state = int_union_real[state - 1][0];
        }

        //gets next character using the while loop
        //1. if character is a digit, adds into token, changes state
        //2. if character is a ., adds into token, changes state
        //3. if character isn't any of the above, puts char back to buffer, checks if it state 2 and categorizes
        //3. into integer, state 4 for real, and if it is not any of these states, then it is unknown (invalid)
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
        //if there is only one character (as in it didn't go through the for loop), gives it the proper state 
        if(state == 2){
            return Token(TokenType::INTEGER, token);
        }
        else if (state == 4) {
            return Token(TokenType::REAL, token);
        }
        else {
            return Token(TokenType::UNKNOWN, token);
        }
    }
};

int main() {

    //get's file name and reads file
    string FILE_NAME;
    cout << "Please enter the file name (LA_input_1.txt , LA_input_2.txt, LA_input_3.txt): ";
    getline(cin, FILE_NAME);

    FILE* filePointer = fopen(FILE_NAME.c_str(), "r");

    if (!filePointer) {
        std::cout << "Error: Cannot open file " << FILE_NAME << std::endl;
        return 1;
    }

    //initializes class and vector
    LexicalAnalyzer la;
    std::vector<Token> tokens;

    //calls lexer and pushes tokens inside the vector
    while (true) {
        Token token = la.lexer(filePointer);
        if (token.value.empty()) {
            break;
        }
        tokens.push_back(token);
    }
    
    //closes file
    fclose(filePointer);

    //get's file name and writes to file
    cout << "Please enter the output file name (LA_output_1.txt , LA_output_2.txt, LA_output_3.txt): ";
    string outputFileName;
    getline(cin, outputFileName);

    ofstream outFile(outputFileName);
    if (!outFile) {
        std::cerr << "Error opening output file for writing" << std::endl;
        return 1;
    }

    outFile << "\nOutput:\n";
    outFile << "                    token                             lexeme\n";
    outFile << "------------------------------------------------------------\n";

    //outputs using the vector
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

        outFile << "                   " << left << setw(35) << type << tokens[i].value << endl;
    }

    return 0;
}
