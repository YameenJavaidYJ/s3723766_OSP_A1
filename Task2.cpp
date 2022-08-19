#include <string>
#include <iostream>
#include <sys/wait.h>
#include <unistd.h>

bool check_filename (const std::string& name);
int map2(const std::string& input, const std::string& output);

int main(int argc, char * argv[]) { 
    if(argv[1] == nullptr || argv[2] == nullptr) {
        std::cout << "Invalid usage: './Task2 INPUTFILE OUTPUTFILE'" << std::endl;
        return 0; 
    }

    //Check if the input file is present and accessible, if no print usage
    if(!check_filename(argv[1])) {
        std::cerr << "File '" << argv[1] << "' not found, usage: './Task2 INPUTFILE OUTPUTFILE'" << std::endl;
        return 0;
    }; 

    std::cout << std::to_string(getpid()) + " | Using input file: " << argv[1] << std::endl;

    //If map2 returns false, there was an error exit
    if(!map2(argv[1], argv[2])) { return 0; }

    std::cout << std::to_string(getpid()) + " | Filtering complete, file '" << argv[2] << "' created!" << std::endl;
}