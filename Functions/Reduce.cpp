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

/*
    Import Utility functions from Utils
*/
bool stringCompareter(std::string s1, std::string s2);
std::string getNextLineFromFile(std::queue<std::string>* stringCache);
void pop_front(std::vector<std::string> &v);
void *readFIFO(void *args);
void *readFIFO4(void *args);
void printLog(std::string print);
void printError(std::string print);

/*
    Define the reduce variables
*/
std::queue<std::string> stringCache[NUMBER_OF_FILES];
std::vector<std::string> sortingCache; 
std::ofstream OutputFile; 

/*
    1. Read all the filtered files produced by mapping and store them into the 13 queues depending on size
    2. Push the first word from each queue into the sortingCache vector
    3. Sort the vector based on 3rd character
    4. Print the first word of the sorted vector to the output file
    5. Pop the printed word from its stringCache queue
    6. Grab the first word of the stringCache queue just popped
    7. Push the word to the sortingCache
    8. Repeat from step 3 until sortingCache is empty
    9. Close the output file and return
*/
int reduce2(const std::string &output) {
    //Steps 1,2,3
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
        sortingCache.push_back(getNextLineFromFile(&stringCache[i])); 
    }

    OutputFile.open(output);

    //Steps 4,5,6,7,8
    while(!sortingCache.empty() && !THREADEXIT) {
        sort(sortingCache.begin(), sortingCache.end(), stringCompareter);
        std::string line = sortingCache.front();
        OutputFile <<  line << "\n"; 
        pop_front(sortingCache); 

        std::string nextLine = getNextLineFromFile(&stringCache[line.length() - ARRAY_OFFSET]); 
        if(!nextLine.empty()) {
            sortingCache.push_back(nextLine);
        }
    }

    //Step 9
    OutputFile.close();
    return 1;
}

/*
    1. Create the 13 threads for reading the FIFO pipes, the threads push the incoming words from map into the stringCache depending on word length
    2. Wait for all the threads to finish, and mapping to signal its complete
    3. Push the first word from each queue into the sortingCache vector
    4. Sort the vector based on 3rd character
    5. Print the first word of the sorted vector to the output file
    6. Pop the printed word from its stringCache queue
    7. Grab the first word of the stringCache queue just popped
    8. Push the word to the sortingCache
    9. Repeat from step 4 until sortingCache is empty
    10. Close the output file and return from the thread
*/
void* reduce3(void* args) {
    FIFOThreadParams params[NUMBER_OF_FILES];
    pthread_t fifoThread[NUMBER_OF_FILES]; 
    struct ReduceThreadParams *mapData = (struct ReduceThreadParams *)args;

    //Step 1
    printLog("Reduce Creating " + std::to_string(NUMBER_OF_FILES) + " threads");
    for (int i = 0; i < NUMBER_OF_FILES; i++)
    {
        params[i].index = i;
        params[i].stringCache = &stringCache[i]; 

        int thread_return = pthread_create(&fifoThread[i], NULL, readFIFO, &params[i]);
        if (thread_return) { return NULL; }
    }

    //Step 2
    for (int i = 0; i < NUMBER_OF_FILES; i++)
    {
        pthread_join(fifoThread[i], NULL);
    }
    printLog("Reduce threads finished"); 

    //Step 2
    printLog("Reduce waiting for mapping signal"); 
    pthread_mutex_lock(&r_mutex);
    while (!REDUCESIGNAL) {
        pthread_cond_wait(&r_cond, &r_mutex);
    }
    printLog("Mapping signal recieved reduce working..."); 

    printLog("Reduce starting merge sort...");
    for (int i = 0; i < NUMBER_OF_FILES && !THREADEXIT; i++)  {
        sortingCache.push_back(getNextLineFromFile(&stringCache[i])); 
    }

    OutputFile.open(mapData->output);

    //Step 4,5,6,7,8,9
    while(!sortingCache.empty() && !THREADEXIT) {
        sort(sortingCache.begin(), sortingCache.end(), stringCompareter);

        std::string line = sortingCache.front();
        OutputFile <<  line << "\n"; 
        pop_front(sortingCache); 

        std::string nextLine = getNextLineFromFile(&stringCache[line.length() - ARRAY_OFFSET]);
        if(!nextLine.empty()) {
            sortingCache.push_back(nextLine);
        }
    }

    //Step 10
    OutputFile.close();
    printLog("## Reducing complete, single reduced filter files in 'Output/Task3' directory as '" + mapData->output + "'");
    return NULL; 
}

/*
    1. Set the threads priority
    2. Create the 13 threads for reading the FIFO pipes, the threads push the incoming words from map into the stringCache depending on word length
    3. Wait for all the threads to finish, and mapping to signal its complete
    4. Push the first word from each queue into the sortingCache vector
    5. Sort the vector based on 3rd character
    6. Print the first word of the sorted vector to the output file
    7. Pop the printed word from its stringCache queue
    8. Grab the first word of the stringCache queue just popped
    9. Push the word to the sortingCache
    10. Repeat from step 5 until sortingCache is empty
    11. Close the output file and return from the thread
*/
void* reduce4(void* args) {
    //Step 1
    nice(-20);

    FIFOThreadParams params[NUMBER_OF_FILES];
    pthread_t fifoThread[NUMBER_OF_FILES]; 
    struct ReduceThreadParams *mapData = (struct ReduceThreadParams *)args;

    //Step 2
    printLog("Reduce Creating " + std::to_string(NUMBER_OF_FILES) + " threads");
    for (int i = 0; i < NUMBER_OF_FILES; i++)
    {
        params[i].index = i;
        params[i].stringCache = &stringCache[i]; 

        int thread_return = pthread_create(&fifoThread[i], NULL, readFIFO4, &params[i]);
        if (thread_return) { return NULL; }
    }

    //Step 3
    for (int i = 0; i < NUMBER_OF_FILES; i++)
    {
        pthread_join(fifoThread[i], NULL);
    }
    printLog("Reduce threads finished"); 

    //Step 3
    printLog("Reduce waiting for mapping signal"); 
    pthread_mutex_lock(&r_mutex);
    while (!REDUCESIGNAL) {
        pthread_cond_wait(&r_cond, &r_mutex);
    }
    printLog("Mapping signal recieved reduce working..."); 

    printLog("Reduce starting merge sort...");
    for (int i = 0; i < NUMBER_OF_FILES && !THREADEXIT; i++)  {
        sortingCache.push_back(getNextLineFromFile(&stringCache[i])); 
    }

    OutputFile.open(mapData->output);

    //Step 5,6,7,8,9,10
    while(!sortingCache.empty() && !THREADEXIT) {
        sort(sortingCache.begin(), sortingCache.end(), stringCompareter);

        std::string line = sortingCache.front();
        OutputFile <<  line << "\n"; 
        pop_front(sortingCache); 

        std::string nextLine = getNextLineFromFile(&stringCache[line.length() - ARRAY_OFFSET]);
        if(!nextLine.empty()) {
            sortingCache.push_back(nextLine);
        }
    }

    //Step 11
    OutputFile.close();
    printLog("## Reducing complete, single reduced filter files in 'Output/Task3' directory as '" + mapData->output + "'");
    return NULL; 
}