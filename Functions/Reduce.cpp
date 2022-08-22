#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include "Commons.h"

const int NUMBER_OF_FILES = 13;
const int ARRAY_OFFSET = 3;

bool stringCompareter(std::string s1, std::string s2);
void pop_front(std::vector<std::string> &v);
void printLog(std::string print);
void printError(std::string print);

std::vector<std::string> readLines; 

int reduce2(const std::string &output) {
    for (int i = 0; i < NUMBER_OF_FILES && !threadExit; i++)
    {
        std::string inputFile = "FilteredFiles/filtered_file_" + std::to_string(i + ARRAY_OFFSET) + ".txt";
        printLog("Reading file '" + inputFile + "' into memory for reduce");
        std::ifstream InputFile(inputFile);
        for (std::string curLine; std::getline(InputFile, curLine);)
        {
            readLines.push_back(curLine); 

        }
        InputFile.close();
    }

    std::ofstream OutputFile(output);

    sort(readLines.begin(), readLines.end(), stringCompareter);
    for(int i = 0; i < (int)readLines.size() && !threadExit; i++) {
        OutputFile << readLines[i] << std::endl; 
    }

    OutputFile.close();
    return 1;
}