#include <iostream>
#include <fstream>
#include <string>
#include "la.hpp"

#define FILE_NAME "source.rat25s"

Token lexer(std::string unparsedText) {
    return Token(TokenType::KEYWORD, "DUMMY TOKEN");
}

int main() {
    std::string line;
    std::fstream sourceCodeFile;

    sourceCodeFile.open(FILE_NAME);

    std::cout << "Token" << "\t\t\t" << "lexeme" << std::endl;
    std::cout << "------------------------------" << std::endl;

    while (std::getline(sourceCodeFile, line)) {
        Token parsedToken = lexer(line);
        std::cout << parsedToken.value << "\t\t" << parsedToken.type << std::endl;
        //call lexer for a token
        //print:
        //      token 
        //      lexeme
    }

    sourceCodeFile.close();
    return 0;
}