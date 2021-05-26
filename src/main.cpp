#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>

#include "../include/interpreter.hpp"

int main(int argc, char *argv[]){
    std::string contents;
    bool isVerbose = false;
    
    // If exactly 2 arguments aren't provided; error.
    if (argc < 2 || argc > 3){
        std::cerr << "bf-interpreter: incorrect amount of arguments provided\nTry 'bf-interpreter --help' for more information.\n";
        return 1;
    } else {
        // Help menu
        if (std::string(argv[1]) == "--help" || std::string(argv[1]) == "-h"){
            std::cerr << "Usage: bf-interpreter -v [FILE]\nInterprets the [FILE] as bf.\n\nFlags:\n\t-h, --help\tshows helpscreen\n\t-v, --verbose\tdescribes every single instruction at runtime\n";
            return 1;
        }
        
        int filenameIndex = argc - 1;

        // Checks if file provided exists, or if the filename corresponds to a directory.
        if (std::filesystem::exists(argv[filenameIndex])){    
            if (std::filesystem::is_directory(argv[filenameIndex])){
                std::cerr << "bf-interpreter: filename provided is that of a directory\nTry 'bf-interpreter --help' for more information.\n";
                return 1;
            }
        } else {
            std::cerr << "bf-interpreter: either the filename provided does not exist, or you didn't put the filename at the end.\n";
            return 1;
        }
        
        // Reads file out, ignoring any characters that don't execute a command.
        std::ifstream file(argv[filenameIndex]);

        char currentCharacter;
        while (file.get(currentCharacter)){
            contents += currentCharacter;
        }

        file.close();

        if (argc == 3 && (std::string(argv[1]) == "-v" || std::string(argv[1]) == "--verbose")){
            isVerbose = true;
        }

        interpret(contents, isVerbose);
    } 
    return 0;
}
