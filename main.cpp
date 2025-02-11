#include <iostream>

#include "la.h"

int main() {
    std::cout << "Lexical Analyzer Running..." << std::endl;

    // place holder
    LexicalAnalyzer lexer;
    std::string input = "int x = 10;";
    lexer.tokenize(input);
    lexer.printTokens();

    return 0;
}
