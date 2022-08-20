#include <iostream>
#include <sys/stat.h>
#include <unistd.h>

void printLog(std::string print);
void printError(std::string print);
std::size_t check_filetype (const std::string& name);
bool check_filename (const std::string& name);
int TaskFilter(const std::string& input, const std::string& output);


int main(int argc, char * argv[]) { 
        //Ensure args present
    if(argv[1] == nullptr || argv[2] == nullptr) {
        printError("Invalid usage: './Task2 INPUTFILE.txt OUTPUTFILE.txt'"); 
        return 0; 
    }
    
    std::string input = argv[1]; 
    std::string output = argv[2]; 

    //Ensure .txt input and outputs
    if(check_filetype(input) == std::string::npos || check_filetype(output) == std::string::npos) {
        printError("File one of the arguments not .txt format, usage: './Task2 INPUTFILE.txt OUTPUTFILE.txt'");
        return 0;
    }; 

    //Check if the input file is present and accessible, if no print usage
    if(!check_filename(input)) {
        printError("File '" + input + "' not found, usage: './Task2 INPUTFILE.txt OUTPUTFILE.txt'");
        return 0;
    }; 

    printLog("Using input file: " + input);

    //If TaskFilter returns false, there was an error filtering
    if(!TaskFilter(input, output)) { printError("Error occured while filtering");  return 0; }
    printLog("Filtering complete, file '" + output + "' created for generic filtered output");
}