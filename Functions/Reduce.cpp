#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>

const int NUMBER_OF_FILES = 13;
const int ARRAY_OFFSET = 3;

bool stringCompareter(std::string s1, std::string s2);
void pop_front(std::vector<std::string> &v);
void printLog(std::string print);
void printError(std::string print);

std::vector<std::string> filteredStreams[NUMBER_OF_FILES];
int linesRead = 0; 

std::vector<std::string> buildVector() { 
    std::vector<std::string> toReturn; 

    for(int i = 0; i < NUMBER_OF_FILES; i++) {
        if(filteredStreams[i].size() > 0)
            toReturn.push_back(filteredStreams[i].front()); 
    }

    return toReturn; 
}

int endOfVec() {
    for(int i = 0; i < NUMBER_OF_FILES; i++) {
        if(filteredStreams[i].size() > 0) { return false; } 
    }

    return true;
}

int reduce2(const std::string &output) {
    for (int i = 0; i < NUMBER_OF_FILES; i++) {
        std::string inputFile = "FilteredFiles/filtered_file_" + std::to_string(i + ARRAY_OFFSET) + ".txt";
        printLog("Reading file '" + inputFile + "' into memory for reduce");
        std::ifstream InputFile(inputFile);
        for (std::string curLine; std::getline(InputFile, curLine);)
        {
            filteredStreams[i].push_back(curLine);
            linesRead++; 
        }
        InputFile.close();
    }

    std::ofstream OutputFile(output); 
    while(linesRead > 0) {
        std::vector<std::string> nextSort = buildVector(); 
        //sort(nextSort.begin(), nextSort.end(), stringCompareter);
        std::string first = nextSort.front(); 

        OutputFile << first << std::endl; 

        pop_front(filteredStreams[first.length() - ARRAY_OFFSET]);
        linesRead--; 
    }

    OutputFile.close();
    return 1;
}