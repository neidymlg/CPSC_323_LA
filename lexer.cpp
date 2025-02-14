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
};

