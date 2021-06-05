#include <filesystem>
#include <fstream>

#include "../include/interpreter.hpp"

int main(int argc, char *argv[]){
    std::string contents;
    std::vector<std::string> flags;
    
    // If less than 2 arguments aren't provided; error.
    if (argc < 2 && argc > 4){
        std::cerr << "bf-interpreter: incorrect amount of arguments provided\nTry 'bf-interpreter --help' for more information.\n";
        return 1;
    } else {
        if (argc == 2 && (std::string(argv[1]) == "-h" || std::string(argv[1]) == "--help")){
            std::cerr << "Example usage: bf-interpreter -v [FILE]\nInterprets the [FILE] as bf.\n\nFlags:\n"
            << "\t-h, --help\tshows helpscreen\n"
            << "\t-v, --verbose\tdescribes every single instruction at runtime\n"
            << "\t-o, --optimize\toptimizes the code before runtime\n"
            << "\t-n, --node-count [NODECOUNT]\tsets the amount of runtime nodes\n"
            << "\t-dw, --disable-wrapping\terrors on overflow/underflow instead of wrapping\n";
            std::exit(0);
        }

        // Checks if file provided exists, or if the filename corresponds to a directory.
        if (std::filesystem::exists(argv[argc - 1])){    
            if (std::filesystem::is_directory(argv[argc - 1])){
                std::cerr << "bf-interpreter: filename provided is that of a directory\nTry 'bf-interpreter --help' for more information.\n";
                return 1;
            }
        } else {
            std::cerr << "bf-interpreter: either the filename provided does not exist, or the filename wasn't provided as the last element.\n";
            return 1;
        }
        
        // Reads file out, ignoring any characters that don't execute a command.
        std::ifstream file(argv[argc - 1]);

        char currentCharacter;
        while (file.get(currentCharacter)){
            contents += currentCharacter;
        }

        file.close();

        if (argc > 2){
            for (int i = 1; i < argc - 1; ++i){
                // Adds everything delimited by a space as a flag, but it's parsed later, so it doesn't matter.
                flags.push_back(std::string(argv[i]));
            }
        }

        interpret(contents, flags);
    } 
    return 0;
}
