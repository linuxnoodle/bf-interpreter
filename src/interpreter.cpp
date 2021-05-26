#include "../include/interpreter.hpp"
#include <climits>

std::vector<int> memory(NODE_COUNT);
unsigned long pointerLocation = NODE_COUNT/2;

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
    int unclosedBrackets = 0;
    for (int i = 0; i < text.length(); ++i){
        if (text[i] == '['){
            ++unclosedBrackets;
        } else if (text[i] == ']'){
            --unclosedBrackets;
        }
    }
    return (unclosedBrackets == 0);
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

int safeChange(int &a, bool incrementOrDecrement){
    if (incrementOrDecrement){
        if (ERROR_OR_WRAP_FOR_OVERFLOWS && a == INT_MAX){
            std::cerr << "bf-interpreter: error: integer overflow\n";
            std::abort();
        } else {
            return ++a;
        }
    } else {
        if (ERROR_OR_WRAP_FOR_OVERFLOWS && a == INT_MIN){
            std::cerr << "bf-interpreter: error: integer underflow\n";
            std::abort();
        } else {
            return --a;
        }
    }
}

unsigned long safePointerChange(unsigned long &a, bool incrementOrDecrement){
    if (incrementOrDecrement){
        if (ERROR_OR_WRAP_FOR_OVERFLOWS && a == NODE_COUNT){
            std::cerr << "bf-interpreter: error: unsigned long overflow\n";
            std::abort();
        } else {
            return ++a;
        }
    } else {
        if (ERROR_OR_WRAP_FOR_OVERFLOWS && a == 0){
            std::cerr << "bf-interpreter: error: unsigned long underflow\n";
            std::abort();
        } else {
            return --a;
        }
    }
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

    std::string buffer = "";

    // Runs every instruction
    for (unsigned long int i = 0; i < cleanedCode.length(); ++i){
        switch (cleanedCode[i]) {
            case '>':
                if (isVerbose)
                    std::cout << "Character " << i << " " << cleanedCode[i] << " position " << pointerLocation << " : Moving pointer to the right.\n";
                safePointerChange(pointerLocation, true);
                break;
            case '<':
                if (isVerbose)
                    std::cout << "Character " << i << " " << cleanedCode[i] << " position " << pointerLocation << " : Moving pointer to the left.\n";
                safePointerChange(pointerLocation, false);
                break;
            case '+':
                if (isVerbose)
                    std::cout << "Character " << i << " " << cleanedCode[i] << " position " << pointerLocation << " : Incrementing value, setting it to the value above " << memory[pointerLocation] << ".\n";
                safeChange(memory[pointerLocation], true);
                break;
            case '-':
                if (isVerbose)
                    std::cout << "Character " << i << " " << cleanedCode[i] << " position " << pointerLocation << " : Decrementing value, setting it to the value below " << memory[pointerLocation] << ".\n";
                safeChange(memory[pointerLocation], false);
                break;
            case '.':
                if (isVerbose){
                    std::cout << "Character " << i << " " << cleanedCode[i] << " position " << pointerLocation << " : Outputting character '" << static_cast<char>(memory[pointerLocation]) << "' to stdout.\n";
                    buffer += static_cast<char>(memory[pointerLocation]); 
                } else {
                    std::cout << static_cast<char>(memory[pointerLocation]);
                }
                break;
            case ',':
                if (isVerbose)
                    std::cout << "Character " << i << " " << cleanedCode[i] << " position " << pointerLocation << " : Taking character from stdin.\n";
                char character;
                std::cin >> character;
                memory[pointerLocation] = static_cast<int>(character);
                break;
            case '[':
                if (memory[pointerLocation] == 0){
                    unsigned long newPosition = findCorrespondingBracket(cleanedCode, i) + 1;
                    if (isVerbose)
                        std::cout << "Character " << i << " " << cleanedCode[i] << " position " << pointerLocation << " : Current node is zero, jumping to " << newPosition << ".\n";
                    if (newPosition > (memory.size() - 1)){
                        std::cerr << "bf-interpreter: fatal error: exceeded memory of " << memory.size() << " nodes\n";
                        std::abort();
                    } else {
                        i = newPosition;
                    }
                } else {
                    if (isVerbose)
                        std::cout << "Character " << i << " " << cleanedCode[i] << " position " << pointerLocation << " : Current node is nonzero, no jumping today.\n";
                }
                break;
            case ']':
                if (memory[pointerLocation] != 0){
                    unsigned long newPosition = findCorrespondingBracket(cleanedCode, i);
                    if (isVerbose)
                        std::cout << "Character " << i << " " << cleanedCode[i] << " position " << pointerLocation << " : Current node is nonzero, jumping back to " << newPosition << ".\n";
                    if (newPosition > (memory.size() - 1)){
                        std::cerr << "bf-interpreter: fatal error: exceeded memory of " << memory.size() << " nodes\n";
                        std::abort();
                    } else {
                        i = newPosition;
                    }
                } else {
                    if (isVerbose)
                        std::cout << "Character " << i << " " << cleanedCode[i] << " position " << pointerLocation << " : Current node is zero, no jumping today.\n";
                }
                break;
        }
    }
    std::cout << buffer;
}
