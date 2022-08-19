#include <string>
#include <iostream>
#include <sys/wait.h>
#include <unistd.h>

std::size_t check_filetype (const std::string& name);
bool check_filename (const std::string& name);
int TaskFilter(const std::string& input, const std::string& output);
int map2(const std::string& input, const std::string& output);
int reduce2(const std::string& output); 

int main(int argc, char * argv[]) { 
    //Ensure args present
    if(argv[1] == nullptr || argv[2] == nullptr) {
        std::cout << "Invalid usage: './Task2 INPUTFILE.txt OUTPUTFILE.txt'" << std::endl;
        return 0; 
    }

    //Ensure .txt input and outputs
    if(check_filetype(argv[1]) == std::string::npos || check_filetype(argv[2]) == std::string::npos) {
        std::cerr << "File one of the arguments not .txt format, usage: './Task2 INPUTFILE.txt OUTPUTFILE.txt'" << std::endl;
        return 0;
    }; 

    //Check if the input file is present and accessible, if no print usage
    if(!check_filename(argv[1])) {
        std::cerr << "File '" << argv[1] << "' not found, usage: './Task2 INPUTFILE.txt OUTPUTFILE.txt'" << std::endl;
        return 0;
    }; 

    std::string input = argv[1]; 
    std::string output = argv[2]; 

    std::cout << std::to_string(getpid()) + " | Using input file: " << input << std::endl;

    //If TaskFilter returns false, there was an error filtering
    if(!TaskFilter(input, output)) { std::cerr << getpid() + " | Error occured while filtering " << "\n"; return 0; }
    std::cout << std::to_string(getpid()) + " | Filtering complete, file '" << output << "' created for generic filtered output" << std::endl;

    //If map2 returns false, there was an error exit
    if(!map2(input, output)) { std::cerr << getpid() + " | Error occured while mapping " << "\n"; return 0; }
    std::cout << std::to_string(getpid()) + " | Mapping complete, filtered files in 'Output' directory" << std::endl;

    //Create a _reduced.txt output, if reduce error exit and output
    std::string reducedOutput = output.replace(output.find(".txt"), output.length(), "_reduced.txt"); 
    if(!reduce2(reducedOutput)) { std::cerr << getpid() + " | Error occured while reducing  " << "\n"; return 0; }
    std::cout << std::to_string(getpid()) + " | Reducing complete, single reduced filter files in Output directory as '" << reducedOutput << "'" << std::endl;

    // Filter, Map, Reduce complete
    std::cout << std::to_string(getpid()) + " | Mapping and reducing complete, file '" << reducedOutput << "' created for mapped and reduced output" << std::endl;
}