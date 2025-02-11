#include <iostream>
#include <fstream>
#include <string>

#include "la.h"

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
        std::cout << parsedToken << std::endl;

        //call lexer for a token
        //print:
        //      token 
        //      lexeme


        
    }
    LexicalAnalyzer la;
    ifstream inputFile("LA_input_1.txt");
    string input;   

    while(inputFile >> input){
        la.analyze(input);
    }
    
    inputFile.close();
    
    la.printTokens();
    
    sourceCodeFile.close();

    return 0;
}
