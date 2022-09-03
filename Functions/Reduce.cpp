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

bool stringCompareter(std::string s1, std::string s2);
void pop_front(std::vector<std::string> &v);
void *readFIFO(void *args);
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
    FIFOThreadParams params[NUMBER_OF_FILES];
    pthread_t fifoThread[NUMBER_OF_FILES]; 
    struct ReduceThreadParams *mapData = (struct ReduceThreadParams *)args;

    printLog("Reduce Creating " + std::to_string(NUMBER_OF_FILES) + " threads");
    for (int i = 0; i < NUMBER_OF_FILES; i++)
    {
        params[i].index = i;
        params[i].stringCache = &stringCache[i]; 

        int thread_return = pthread_create(&fifoThread[i], NULL, readFIFO, &params[i]);
        if (thread_return) { return NULL; }
    }

    for (int i = 0; i < NUMBER_OF_FILES; i++)
    {
        pthread_join(fifoThread[i], NULL);
    }
    printLog("Reduce threads finished"); 

    printLog("Reduce waiting for mapping signal"); 
    pthread_mutex_lock(&r_mutex);
    while (!REDUCESIGNAL) {
        pthread_cond_wait(&r_cond, &r_mutex);
    }
    printLog("Mapping signal recieved reduce working..."); 

    printLog("Reduce starting merge sort...");
    for (int i = 0; i < NUMBER_OF_FILES && !THREADEXIT; i++)  {
        sortingCache.push_back(getNextLineFromFile(i)); 
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