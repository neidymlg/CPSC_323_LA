#pragma once
#include <iostream>
#include <string>
#include <fstream>

enum class TokenType {
    KEYWORD, //complete
    IDENTIFIER, //complete
    INTEGER, //complete
    REAL,
    OPERATOR, //complete
    SEPARATOR, //complete
    UNKNOWN //complete
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

struct Token {
    TokenType type;
    std::string value;

    Token(TokenType type, std::string value);
    Token();
};

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

Token lexer(FILE* filePointer) {
    // char myChar = getc(filePointer);
    LexerState state = LexerState::START;
    std::string tokenString = "";
    char c;

    TokenType myTokenType;
    while(true) {
        c = getc(filePointer);

        switch(state) {
            case LexerState::START:
                if(isWhiteSpace(c)) {
                    state = LexerState::START;
                } else if(isKeyword(c)) {
                    state = LexerState::KEYWORD;
                } else {
                    state = LexerState::UNKNOWN;
                }
                break;
            case LexerState::KEYWORD:
                if(isKeyword(c)) {
                    state = LexerState::KEYWORD;
                } else {
                    state = LexerState::UNKNOWN;
                }
                break;
            case LexerState::UNKNOWN:
                break;
    }
        //if(isWhiteSpace(myChar)) {
            //change state
        //     //go forward one char
        //     myChar = getc(filePointer);
        //     break;
        // } else if(isKeyword(myChar)){
        //     break;
        // } else {
        //     //error
        //     break;
        // }
    // }
    
    
    
    // std::string tokenString = std::string(1, myChar);
    // return Token(TokenType::IDENTIFIER, tokenString);
}
};

