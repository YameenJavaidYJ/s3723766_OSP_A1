#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>

std::ofstream* outputFiles[13]; 
std::vector<std::string> inputStreams[13]; 

bool comparator(std::string s1, std::string s2) {
    if(s1.length() == 0) { return false; }
    if(s2.length() == 0) { return true; }

    return s1.substr(2, std::string::npos) < s2.substr(2, std::string::npos);
}

int map2(const std::string& input, const std::string& output) {
    std::cout << std::to_string(getpid()) + " | Filtering complete, file '" << output << "' created!" << std::endl;
    std::cout << std::to_string(getpid()) + " | Creating the individual output files" << std::endl;

    for(int i = 0; i <= 12; i++) {
        std::string outputFile = "FilteredFiles/filter_file_" + std::to_string(i+3) + ".txt";
        outputFiles[i] = new std::ofstream(outputFile, std::ofstream::trunc);
        std::cout << std::to_string(getpid()) + " | filter_file_" + std::to_string(i+3) + " created" << std::endl; 
    }

    std::ifstream InputFile(output); 
    
    for(std::string curLine; std::getline(InputFile, curLine);) {
        inputStreams[curLine.length() - 3].push_back(curLine);
    }

    for(int i = 0; i<=12; i++) {
        if(fork() == 0) {
            std::cout << std::to_string(getpid()) + " | Sorting strings of length " + std::to_string(i+3) << std::endl;
            sort(inputStreams[i].begin(), inputStreams[i].end(), comparator);
            std::cout << std::to_string(getpid()) + " | Sorting complete for length " + std::to_string(i+3) << std::endl;
            for(std::string str: inputStreams[i]) {
                *outputFiles[i] << str << "\n"; 
            }
            std::cout << std::to_string(getpid()) + " | Written to filter_file_" + std::to_string(i+3) + ".txt" << std::endl;
            exit(0);
        }
    }

    while(wait(NULL) != -1 || errno != ECHILD); 

    for(int i = 0; i <= 12; i++) {
        outputFiles[i]->close(); 
    }

    InputFile.close(); 

    return 1;
}