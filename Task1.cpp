#include <iostream>
#include <sys/stat.h>
#include <unistd.h>

std::size_t check_filetype (const std::string& name);
bool check_filename (const std::string& name);
int TaskFilter(const std::string& input, const std::string& output);


int main(int argc, char * argv[]) { 
    //Ensure args present
    if(argv[1] == nullptr || argv[2] == nullptr) {
        std::cout << "Invalid usage: './Task1 INPUTFILE.txt OUTPUTFILE.txt'" << std::endl;
        return 0; 
    }

    //Ensure .txt input and outputs
    if(check_filetype(argv[1]) == std::string::npos || check_filetype(argv[2]) == std::string::npos) {
        std::cerr << "File one of the arguments not .txt format, usage: './Task1 INPUTFILE.txt OUTPUTFILE.txt'" << std::endl;
        return 0;
    }; 

    //Check if the input file is present and accessible, if no print usage
    if(!check_filename(argv[1])) {
        std::cerr << "File '" << argv[1] << "' not found, usage: './Task1 INPUTFILE.txt OUTPUTFILE.txt'" << std::endl;
        return 0;
    }; 

    std::cout << "Using input file: " << argv[1] << std::endl;

    //If TaskFilter returns false, there was an error filtering
    if(!TaskFilter(argv[1], argv[2])) { std::cerr << std::to_string(getpid()) << " | There was an error filtering" << std::endl;  return 0; }
    std::cout << "Filtering complete, file '" << argv[2] << "' created!" << std::endl;
}