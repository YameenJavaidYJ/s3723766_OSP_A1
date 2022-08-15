#include <iostream>

int TaskFilter(const std::string& input, const std::string& output);

int main(int argc, char * argv[]) { 
    if(argv[1] == nullptr || argv[2] == nullptr) {
        std::cout << "Invalid usage: './Task1 INPUTFILE OUTPUTFILE'" << std::endl;
        return 0; 
    }

    std::cout << "Using input file: " << argv[1] << std::endl;
    if(!TaskFilter(argv[1], argv[2])) { return 0; }

    std::cout << "Filtering complete, file '" << argv[2] << "' created!" << std::endl;
}