#include <iostream>
#include <vector>
#include <fstream>
#include <string>

#include "la.h"

#define FILE_NAME "source.rat25s"

std::string tokenTypeToString(TokenType type) {
    switch (type) {
        case TokenType::KEYWORD: return "KEYWORD";
        // Add other cases for different token types
        default: return "UNKNOWN";
    }
}


class Lexer {
public:
    std::vector<Token> tokens;

    void analyze(const std::string& unparsedText) {
        // Dummy implementation
        tokens.push_back(Token(TokenType::KEYWORD, "DUMMY TOKEN"));
    }

    const std::vector<Token>& getTokens() const {
        return tokens;
    }
};

Lexer lexer;

int main() {
    std::string line;
    std::fstream sourceCodeFile;

    sourceCodeFile.open(FILE_NAME);

    std::cout << "Token" << "\t\t\t" << "lexeme" << std::endl;
    std::cout << "------------------------------" << std::endl;

    while (std::getline(sourceCodeFile, line)) {
        Token parsedToken = lexer.analyze(line);

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
    lexer.analyze("input.txt");
    // sourceCodeFile.close();











    // ofstream output("output.txt");
    // if (!output.is_open()) {
    //     cerr << "Error opening output file!" << endl;
    //     return 1;
    // }

    // for (const Token& token : lexer.getTokens()) {
    //     output << "<" << tokenTypeToString(token.type) << ", " << token.value << ">\n";
    // }

    // output.close();

    return 0;
}
