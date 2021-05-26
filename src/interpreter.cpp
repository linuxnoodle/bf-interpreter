#include "../include/interpreter.hpp"

std::vector<int> memory(30000);
unsigned long int pointerLocation = 15000;

std::string clean(std::string dirtyCode){
    std::string cleanedCode;
    for (int i = 0; i < dirtyCode.length(); ++i){    
        switch (dirtyCode[i]) {
            case '>':
                cleanedCode += '>';
                break;
            case '<':
                cleanedCode += '<';
                break;
            case '+':
                cleanedCode += '+';
                break;
            case '-':
                cleanedCode += '-';
                break;
            case '.':
                cleanedCode += '.';
                break;
            case ',':
                cleanedCode += ',';
                break;
            case '[':
                cleanedCode += '[';
                break;
            case ']':
                cleanedCode += ']';
                break;
            default:
                break;
        }
    }
    return cleanedCode; 
}

bool checkIfBracketsAreBalanced(std::string text){
    int leftCount = 0, rightCount = 0;
    for (int i = 0; i < text.length(); ++i){
        if (text[i] == '['){
            ++leftCount;
        } else if (text[i] == ']'){
            ++rightCount;
        }
    }
    return (leftCount == rightCount);
}

int findCorrespondingBracket(std::string text, int bracketPosition){
    int correspondingPosition = bracketPosition;
    int counter = 1;
    int weight = (text[bracketPosition] == '[') ? 1 : -1;
    while (counter > 0){
        char character = text[correspondingPosition += weight];
        if (character == '['){
            counter += weight;
        } else if (character == ']'){
            counter -= weight;
        }
    }
    return correspondingPosition;
}
void interpret(std::string code, bool isVerbose){
    if (isVerbose)
        std::cout << "Generating memory.\n";
    std::fill(memory.begin(), memory.end(), 0);
    if (isVerbose)
        std::cout << "Filtering input code of comments.\n";
    std::string cleanedCode = clean(code);
    
    if (!checkIfBracketsAreBalanced(cleanedCode)){
        std::cerr << "bf-interpreter: fatal error: provided code has unbalanced brackets\n";
    }

    for (unsigned long int i = 0; i < cleanedCode.length(); ++i){
        switch (cleanedCode[i]) {
            case '>':
                if (isVerbose)
                    std::cout << "Character " << i << ", " << cleanedCode[i] << ": Moving pointer to position " << pointerLocation << " in memory.\n";
                ++pointerLocation;
                break;
            case '<':
                if (isVerbose)
                    std::cout << "Character " << i << ", " << cleanedCode[i] << ": Moving pointer to position " << pointerLocation << " in memory.\n";
                --pointerLocation;
                break;
            case '+':
                if (isVerbose)
                    std::cout << "Character " << i << ", " << cleanedCode[i] << ": Incrementing value at position " << pointerLocation << " in memory, setting it to value " << memory[pointerLocation] << ".\n";
                ++memory[pointerLocation];
                break;
            case '-':
                if (isVerbose)
                    std::cout << "Character " << i << ", " << cleanedCode[i] << ": Decrementing value at position " << pointerLocation << " in memory, setting it to value " << memory[pointerLocation] << ".\n";
                --memory[pointerLocation];
                break;
            case '.':
                if (isVerbose)
                    std::cout << "Character " << i << ", " << cleanedCode[i] << ": Outputting character '" << static_cast<char>(memory[pointerLocation]) << "' to stdout.\n";
                std::cout << static_cast<char>(memory[pointerLocation]);
                break;
            case ',':
                if (isVerbose)
                    std::cout << "Character " << i << ", " << cleanedCode[i] << ": Taking character from stdin and storing it in memory position " << pointerLocation << ".\n";
                char character;
                std::cin >> character;
                memory[pointerLocation] = static_cast<int>(character);
                break;
            case '[':
                if (memory[pointerLocation] == 0){
                    unsigned long int newPosition = findCorrespondingBracket(cleanedCode, i) + 1;
                    if (isVerbose)
                        std::cout << "Character " << i << ", " << cleanedCode[i] << ": Memory at position " << pointerLocation << " is zero, jumping to " << newPosition << ".\n";
                    if (newPosition > 29999){
                        std::cerr << "bf-interpreter: fatal error: exceeded memory of 30,000 nodes\n";
                        std::abort();
                    } else {
                        i = newPosition;
                    }
                } else {
                    if (isVerbose)
                        std::cout << "Character " << i << ", " << cleanedCode[i] << ": Memory at position " << pointerLocation << " is nonzero, no jumping today.\n";
                }
                break;
            case ']':
                if (memory[pointerLocation] != 0){
                    unsigned long int newPosition = findCorrespondingBracket(cleanedCode, i);
                    if (isVerbose)
                        std::cout << "Character " << i << ", " << cleanedCode[i] << ": Memory at position " << pointerLocation << " is nonzero, jumping back to " << newPosition << ".\n";
                    if (newPosition > 29999){
                        std::cerr << "bf-interpreter: fatal error: exceeded memory of 30,000 nodes\n";
                        std::abort();
                    } else {
                        i = newPosition;
                    }
                } else {
                    if (isVerbose)
                        std::cout << "Character " << i << ", " << cleanedCode[i] << ": Memory at position " << pointerLocation << " is zero, no jumping today.\n";
                }
                break;
        }
    }
}
