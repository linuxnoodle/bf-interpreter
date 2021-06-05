#include "../include/interpreter.hpp"

std::vector<int> memory;
int pointerLocation;
struct flags{
    bool isVerbose = false;
    bool isOptimized = false;
    bool wrapOrError = false;
    int nodeCount = 30000;
} runtimeFlags;

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

template <typename T>
T safeChange(T a, T b, T maximumValue, T minimumValue){
    if (((b > 0) && (a > (maximumValue - b)))){
        if (runtimeFlags.wrapOrError){
            std::cerr << "bf-interpreter: error: value overflow\n";
            std::abort();
        } else {
            return minimumValue;        
        }
    } else if ((b < 0) && (a < (minimumValue - b))){
        if (runtimeFlags.wrapOrError){
            std::cerr << "bf-interpreter: error: value underflow\n";
            std::abort();
        } else {
            return maximumValue;
        }
    } else {
        return a + b;
    }
}

void movePointer(int amount, int index){
    if (runtimeFlags.isVerbose)
        std::cout << "Character " << index << " " << ((amount > 0) ? ">" : "<") << "  Position " << pointerLocation << " : Moving pointer " << amount << ".\n";
    pointerLocation = safeChange<int>(pointerLocation, amount, runtimeFlags.nodeCount - 1, 0);
}

void changeCell(int amount, int index){
    // Underlying type is an INT32, but with safeChange, I am giving it the boundaries of a UCHAR.
    if (runtimeFlags.isVerbose)
        std::cout << "Character " << index << ((amount > 0) ? " + " : " - ") << " Position " << pointerLocation << " : " << "Adding " << amount << ", setting it to the value " << static_cast<int>(safeChange<int>(memory[pointerLocation], amount, UCHAR_MAX, 0)) << ".\n";
    memory[pointerLocation] = safeChange<int>(memory[pointerLocation], amount, UCHAR_MAX, 0);
}

void ioCell(bool isInputOrOutput, int index){
    if (isInputOrOutput){
        if (runtimeFlags.isVerbose){
            std::cout << "Character " << index << " . Position " << pointerLocation << " : Outputting character with keycode '" << memory[pointerLocation] << "' to stdout.\n";
        } else {
            std::cout << static_cast<char>(memory[pointerLocation]);
        }
    } else {
        if (runtimeFlags.isVerbose)
            std::cout << "Character " << index << " , Position " << pointerLocation << " : Taking character from stdin.\n";
        char character;
        std::cin >> character;
        memory[pointerLocation] = static_cast<int>(character);
    } 
}

void interpret(std::string code, std::vector<std::string> flags){
    for (int i = 0; i < flags.size(); ++i){
        if (flags[i] == "-v" || flags[i] == "--verbose"){
            runtimeFlags.isVerbose = true;
        } else if (flags[i] == "-o" || flags[i] == "--optimized"){
            runtimeFlags.isOptimized = true;
        } else if (flags[i] == "-n" || flags[i] == "--node-count"){
            if (i != flags.size() - 1){
                try {
                     runtimeFlags.nodeCount = std::stoi(flags[i + 1]);
                     std::cout << runtimeFlags.nodeCount << "\n";
                } catch (std::invalid_argument){
                    std::cerr << "bf-interpreter: error: non-integer provided to node count flag\n";
                }                
            } else {
                std::cerr << "bf-interpreter: error: node count flag used, but no number was actually provided\n";
            }
        } else if (flags[i] == "-dw" || flags[i] == "--disable-wrapping"){
            runtimeFlags.wrapOrError = true;
        }
    }

    memory.resize(runtimeFlags.nodeCount);
    std::fill(memory.begin(), memory.end(), 0);
    pointerLocation = runtimeFlags.nodeCount/2;

    if (runtimeFlags.isVerbose)
        std::cout << "Filtering input code of comments.\n";
    std::string cleanedCode = clean(code);
    
    if (!checkIfBracketsAreBalanced(cleanedCode)){
        std::cerr << "bf-interpreter: error: provided code has unbalanced brackets\n";
    }

    // Runs every instruction
    for (unsigned long int i = 0; i < cleanedCode.length(); ++i){
        switch (cleanedCode[i]) {
            case '>':
                if (runtimeFlags.isOptimized){
                    bool hasNotHitDifferentChar = true;
                    int amount = 1;
                    while (i != cleanedCode.length() - 1 && hasNotHitDifferentChar){
                        if (cleanedCode[i + amount] == '>'){
                            ++amount;
                            ++i;
                        } else {
                            hasNotHitDifferentChar = false;
                        }
                    }
                    movePointer(amount, i);
                } else {
                    movePointer(1, i);
                }
                break;
            case '<':
                if (runtimeFlags.isOptimized){
                    bool hasNotHitDifferentChar = true;
                    int amount = -1;
                    while (i != cleanedCode.length() - 1 && hasNotHitDifferentChar){
                        if (cleanedCode[i - amount] == '>'){
                            --amount;
                            ++i;
                        } else {
                            hasNotHitDifferentChar = false;
                        }
                    }
                    movePointer(amount, i);
                } else {
                    movePointer(-1, i);
                }
                break;
            case '+':
                if (runtimeFlags.isOptimized){
                    bool hasNotHitDifferentChar = true;
                    int amount = 1;
                    while (i != cleanedCode.length() - 1 && hasNotHitDifferentChar){
                        if (cleanedCode[i - amount] == '+'){
                            ++amount;
                            ++i;
                        } else {
                            hasNotHitDifferentChar = false;
                        }
                    }
                    changeCell(amount, i);
                } else {
                    changeCell(1, i);
                }
                break;
            case '-':
                if (runtimeFlags.isOptimized){
                    bool hasNotHitDifferentChar = true;
                    int amount = -1;
                    while (i != cleanedCode.length() - 1 && hasNotHitDifferentChar){
                        if (cleanedCode[i - amount] == '-'){
                            --amount;
                            ++i;
                        } else {
                            hasNotHitDifferentChar = false;
                        }
                    }
                    changeCell(amount, i);
                    i -= amount;
                } else {
                    changeCell(-1, i);
                }
                break;
            case '.':
                ioCell(true, i);
                break;
            case ',':
                ioCell(false, i);
                break;
            case '[':
                if (memory[pointerLocation] == 0){
                    unsigned long newPosition = findCorrespondingBracket(cleanedCode, i);
                    if (runtimeFlags.isVerbose)
                        std::cout << "Character " << i << " " << cleanedCode[i] << " Position " << pointerLocation << " : Current node is zero, jumping to " << newPosition << ".\n";
                    if (newPosition > (runtimeFlags.nodeCount - 1)){
                        std::cerr << "bf-interpreter: fatal error: exceeded memory of " << runtimeFlags.nodeCount << " nodes\n";
                        std::abort();
                    } else {
                        i = newPosition;
                    }
                } else {
                    if (runtimeFlags.isVerbose)
                        std::cout << "Character " << i << " " << cleanedCode[i] << " Position " << pointerLocation << " : Current node is nonzero, no jumping today.\n";
                }
                break;
            case ']':
                if (memory[pointerLocation] != 0){
                    unsigned long newPosition = findCorrespondingBracket(cleanedCode, i);
                    if (runtimeFlags.isVerbose)
                        std::cout << "Character " << i << " " << cleanedCode[i] << " Position " << pointerLocation << " : Current node is nonzero, jumping back to " << newPosition << ".\n";
                    if (newPosition > (runtimeFlags.nodeCount - 1)){
                        std::cerr << "bf-interpreter: fatal error: exceeded memory of " << runtimeFlags.nodeCount << " nodes\n";
                        std::abort();
                    } else {
                        i = newPosition;
                    }
                } else {
                    if (runtimeFlags.isVerbose)
                        std::cout << "Character " << i << " " << cleanedCode[i] << " Position " << pointerLocation << " : Current node is zero, no jumping today.\n";
                }
                break;
        }
    }
}
