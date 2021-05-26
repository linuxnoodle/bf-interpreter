#include <iostream>
#include <string>
#include <fstream>

int main(){
    std::cout << "Text to print:\n";
    
    std::string input;
    std::getline(std::cin, input);
    input += "\n";

    std::string code = "";

    for (int i = 0; i < input.length(); ++i){
        for (int j = 0; j < static_cast<int>(input[i]); ++j){
            code += "+";
        }    
        code += ".>";
    }

    std::ofstream output;
    output.open("output.b");
    output << code;
    output.close();
    return 0;
}
