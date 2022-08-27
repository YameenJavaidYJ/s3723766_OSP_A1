#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include "Commons.h"

const int NUMBER_OF_FILES = 13;
const int ARRAY_OFFSET = 3;

bool stringCompareter(std::string s1, std::string s2);
void pop_front(std::vector<std::string> &v);
void printLog(std::string print);
void printError(std::string print);

std::queue<std::string> stringCache[NUMBER_OF_FILES];
std::vector<std::string> sortingCache; 
std::ofstream OutputFile; 

std::string getNextLineFromFile(int fileIndex) {
    std::string line; 

    if(stringCache[fileIndex].size() == 0) {
        return "";
    }

    line = stringCache[fileIndex].front();
    stringCache[fileIndex].pop(); 

    return line; 
}

int reduce2(const std::string &output) {
    for (int i = 0; i < NUMBER_OF_FILES && !threadExit; i++)
    {
        std::ifstream inputStream;
        std::string inputFile = "FilteredFiles/filtered_file_" + std::to_string(i + ARRAY_OFFSET) + ".txt";
        printLog("Reading file '" + inputFile + "' into memory for reduce");
        inputStream.open(inputFile); 
        
        for(std::string line; std::getline(inputStream, line);){
            stringCache[i].push(line); 
        }

        inputStream.close();
        sortingCache.push_back(getNextLineFromFile(i)); 
    }

    OutputFile.open(output);

    while(!sortingCache.empty() && !threadExit) {
        sort(sortingCache.begin(), sortingCache.end(), stringCompareter);
        std::string line = sortingCache.front();
        OutputFile <<  line << "\n"; 
        pop_front(sortingCache); 

        std::string nextLine = getNextLineFromFile(line.length() - ARRAY_OFFSET); 
        if(!nextLine.empty()) {
            sortingCache.push_back(nextLine);
        }
    }

    OutputFile.close();
    return 1;
}