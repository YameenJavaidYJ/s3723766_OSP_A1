#include <iostream>
#include <sys/stat.h>
#include <unistd.h>

int TaskFilter(const std::string& input, const std::string& output);
bool check_filename (const std::string& name);


int main(int argc, char * argv[]) { 
    if(argv[1] == nullptr || argv[2] == nullptr) {
        std::cout << "Invalid usage: './Task1 INPUTFILE OUTPUTFILE'" << std::endl;
        return 0; 
    }

    //Check if the input file is present and accessible, if no print usage
    if(!check_filename(argv[1])) {
        std::cerr << "File '" << argv[1] << "' not found, usage: './Task1 INPUTFILE OUTPUTFILE'" << std::endl;
        return 0;
    }; 

    std::cout << "Using input file: " << argv[1] << std::endl;

    //If TaskFilter returns false, there was an error filtering
    if(!TaskFilter(argv[1], argv[2])) { return 0; }

    std::cout << "Filtering complete, file '" << argv[2] << "' created!" << std::endl;
}