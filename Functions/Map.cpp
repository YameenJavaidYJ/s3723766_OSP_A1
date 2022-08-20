#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>

const int ARRAY_SIZE = 13; 
const int ARRAY_OFFSET = 3; 

bool stringCompareter(std::string s1, std::string s2);
void printLog(std::string print);
void printError(std::string print);

std::ofstream* outputFiles[ARRAY_SIZE]; 
std::vector<std::string> inputStreams[ARRAY_SIZE]; 

int map2(const std::string& input, const std::string& output) {
    printLog("Creating the individual output files");

    for(int i = 0; i < ARRAY_SIZE; i++) {
        std::string outputFile = "FilteredFiles/filtered_file_" + std::to_string(i+ARRAY_OFFSET) + ".txt";
        outputFiles[i] = new std::ofstream(outputFile, std::ofstream::trunc);
        printLog("filter_file_" + std::to_string(i+ARRAY_OFFSET) + " created"); 
    }

    std::ifstream InputFile(output); 
    
    for(std::string curLine; std::getline(InputFile, curLine);) {
        inputStreams[curLine.length() - ARRAY_OFFSET].push_back(curLine);
    }

    for(int i = 0; i < ARRAY_SIZE; i++) {
        if(fork() == 0) {
            printLog("Sorting strings of length " + std::to_string(i+ARRAY_OFFSET));
            sort(inputStreams[i].begin(), inputStreams[i].end(), stringCompareter);
            printLog("Sorting complete for length " + std::to_string(i+ARRAY_OFFSET));
            for(std::string str: inputStreams[i]) {
                *outputFiles[i] << str << "\n"; 
            }
            printLog("Written to filter_file_" + std::to_string(i+ARRAY_OFFSET) + ".txt");
            exit(0);
        }
    }

    while(wait(NULL) != -1 || errno != ECHILD); 

    for(int i = 0; i < ARRAY_SIZE; i++) {
        outputFiles[i]->close(); 
    }

    InputFile.close(); 

    return 1;
}