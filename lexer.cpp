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
};

bool isWhiteSpace(char c) {
    if(c == ' ') {
        return true;
    }
    return false;
}

Token lexer(FILE* filePointer) {
    char myChar = getc(filePointer);
    
    if(isWhiteSpace(myChar)) {
        //change state
    }
    
    
    std::string tokenString = std::string(1, myChar);
    return Token(TokenType::IDENTIFIER, tokenString);
};

