#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <unistd.h>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include "Commons.h"

const int NUMBER_OF_FILES = 13;
const int ARRAY_OFFSET = 3;

bool stringCompareter(std::string s1, std::string s2);
void pop_front(std::vector<std::string> &v);
int countOfStringsLength(int length);
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
    for (int i = 0; i < NUMBER_OF_FILES && !THREADEXIT; i++)
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

    while(!sortingCache.empty() && !THREADEXIT) {
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

void* reduce3(void* args) {
    int fifoHandles[NUMBER_OF_FILES]; 
    int arraySizes[NUMBER_OF_FILES]; 
    struct ReduceThreadParams *mapData = (struct ReduceThreadParams *)args;

    printLog("Creating/Openning reduce FIFO Files");
    for(int i = 0; i < NUMBER_OF_FILES; i++) {
        if(mkfifo(("FIFOFiles/fifo_file_" + std::to_string(i + ARRAY_OFFSET)).c_str(), 0777) == -1) {
            if(errno != EEXIST) {
                printError("Could not create FIFO file for index: " + std::to_string(i+ARRAY_OFFSET));
                return NULL; 
            }
        }
        
        fifoHandles[i] = open(("FIFOFiles/fifo_file_" + std::to_string(i + ARRAY_OFFSET)).c_str(), O_RDONLY);
        if(fifoHandles[i] == -1) { return NULL; }
    }

    for(int i = 0; i < NUMBER_OF_FILES; i++) {
        arraySizes[i] = countOfStringsLength(i+ARRAY_OFFSET); 
    }

    printLog("Reduce waiting for mapping signal"); 
    pthread_mutex_lock(&r_mutex);
    while (!REDUCESIGNAL) {
        pthread_cond_wait(&r_cond, &r_mutex);
    }
    printLog("Mapping signal recieved reduce working..."); 

    printLog("Reading FIFO files into memory for reduce");
    for (int i = 0; i < NUMBER_OF_FILES && !THREADEXIT; i++)
    {
        int handle = fifoHandles[i]; 
        int pipeLength = arraySizes[i]; 
        int fifoRead[pipeLength]; 

        if(read(handle, fifoRead, sizeof(int)*pipeLength) == -1) {
            printError("There was an error reading from FIFO " + std::to_string(i));
            return NULL; 
        }


        for(int j = 0; j < pipeLength; j++) {
            stringCache[i].push(TASK3_GLOBALSTRINGS.at(fifoRead[j])); 
        }

        sortingCache.push_back(getNextLineFromFile(i)); 

        close(fifoHandles[i]);
    }

    OutputFile.open(mapData->output);

    while(!sortingCache.empty() && !THREADEXIT) {
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
    printLog("## Reducing complete, single reduced filter files in 'Output/Task3' directory as '" + mapData->output + "'");
    return NULL; 
}