#include <string>
#include <fstream>
#include <iostream>
#include <vector>

const int NUMBER_OF_FILES = 13;
const int ARRAY_OFFSET = 3;

void printLog(std::string print);
void printError(std::string print);

std::vector<std::string> filteredStreams[NUMBER_OF_FILES];

std::vector<std::string> buildVector() { 
    std::vector<std::string> toReturn; 

    for(int i = 0; i < NUMBER_OF_FILES; i++) {
        toReturn.push_back(filteredStreams[i].front()); 
    }

    return toReturn; 
}

int reduce2(const std::string &output) {
    for (int i = 0; i < NUMBER_OF_FILES; i++) {
        std::string inputFile = "FilteredFiles/filtered_file_" + std::to_string(i + ARRAY_OFFSET) + ".txt";
        printError("Reading file '" + inputFile + "' into memory for reduce");
        std::ifstream InputFile(inputFile);
        for (std::string curLine; std::getline(InputFile, curLine);)
        {
            filteredStreams[i].push_back(curLine);
        }
        InputFile.close();
    }

    


    return 1;
}