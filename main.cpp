#include <iostream>
#include <vector>
#include <fstream>
#include <string>

#include "lexer.cpp"

#define FILE_NAME "source.rat25s"

int main() {

    //open the file and store a pointer acting as a cursor.
    FILE *filePointer = fopen(FILE_NAME, "r");

    if(filePointer == nullptr) {
        std::cout << "Whoops! Looks like there was an error finding the specified file." << std::endl;
        return 1;
    } else {
        std::cout << "Opened File: " << FILE_NAME << std::endl;
    }
    

    //get each char from the file one by one.
    //  - getc(filePointer) to get the next char.
    //  - unget(1, filePointer) to move the cursor back one.
    while(true) {
        try {
            Token myToken = lexer(filePointer);
            std::cout << myToken.value << std::endl;
        } catch (std::string error) {
            std::cout << error << std::endl;
            break;
        }
        if(feof(filePointer)) {
            break;
        }

    }

    //close the file.
    auto fileCloseError = fclose(filePointer);
    if(!fileCloseError) {
        std::cout << "Closed File: " << FILE_NAME << std::endl;
    } else {
        std::cout << "Error closing file: " << FILE_NAME << std::endl;
        return 1;
    }
    return 0;
}
