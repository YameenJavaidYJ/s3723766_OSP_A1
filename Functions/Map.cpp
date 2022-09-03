#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "Commons.h"

/*
    Import Utility functions from Utils
*/
bool stringCompareter(std::string s1, std::string s2);
bool stringCompareterTask3(int i1, int i2);
void printLog(std::string print);
void printError(std::string print);
void *writeFIFO(void *args);
void *writeFIFO4(void *args);

/*
    1. Read the Task1Filter input file into the 13 vectors based on word length (length 3 goes into vector 0 etc...)
    2. Fork the parent process 13 times, each fork creates its filter_length file and writes the contents of its vector to the file
    3. Close the fork and return
    4. Wait for all the forks to finish
    5. Return from the function 
*/
int map2(const std::string &input, const std::string &output)
{
    std::vector<std::string> inputStreams[NUMBER_OF_FILES];
    std::ifstream InputFile;

    //Step 1
    InputFile.open(output);

    for (std::string curLine; std::getline(InputFile, curLine);)
    {
        inputStreams[curLine.length() - ARRAY_OFFSET].push_back(curLine);
    }

    InputFile.close();

    //Step 2
    for (int i = 0; i < NUMBER_OF_FILES && !THREADEXIT; i++)
    {
        if (fork() == 0)
        {
            printLog("Sorting strings of length " + std::to_string(i + ARRAY_OFFSET));
            sort(inputStreams[i].begin(), inputStreams[i].end(), stringCompareter);
            printLog("Sorting complete for length " + std::to_string(i + ARRAY_OFFSET));

            std::ofstream OutputFile;
            std::string outputFile = "FilteredFiles/filtered_file_" + std::to_string(i + ARRAY_OFFSET) + ".txt";
            OutputFile.open(outputFile, std::ofstream::trunc);
            printLog("filter_file_" + std::to_string(i + ARRAY_OFFSET) + " created");
            for (std::string str : inputStreams[i])
            {
                OutputFile << str << "\n";
            }
            printLog("Written to filter_file_" + std::to_string(i + ARRAY_OFFSET) + ".txt");

            //Step 3
            OutputFile.close();
            printLog("filter_file_" + std::to_string(i + ARRAY_OFFSET) + " closed, fork finished");
            exit(0);
        }
    }

    //Step 4
    while (wait(NULL) != -1 || errno != ECHILD);

    //Step 5
    printLog("Mapping forks finished");
    return 1;
}

/*
    1. Create an index'd array of vectors based on the GLOBAL array produced at the start of task 3
    2. Sort each of the index arrays based on 3rd character
    3. Create the 13 threads for the FIFO files
    4. Wait for all threads to finish writing to their pipes
    5. Signal reduce to start the merge sort now all the words have been transfered
    6. Return from the mapping thread
*/
void *map3(void *args)
{
    std::vector<std::vector<int>> indexArray(13, std::vector<int>());
    MapThreadParams params[NUMBER_OF_FILES];
    pthread_t fifoThread[NUMBER_OF_FILES];

    //Step 1
    printLog("Created index'd array");
    int counter = 0;
    for (std::string string : TASK3_GLOBALSTRINGS)  {
        int length = string.length();
        indexArray.at(length - ARRAY_OFFSET).push_back(counter);
        counter++;
    }

    //Step 2
    for(int i = 0; i < NUMBER_OF_FILES; i++) {
        sort(indexArray.at(i).begin(), indexArray.at(i).end(), stringCompareterTask3); 
    }

    //Step 3
    printLog("Map Creating " + std::to_string(NUMBER_OF_FILES) + " threads");
    for (int i = 0; i < NUMBER_OF_FILES; i++)
    {
        params[i].index = i; 
        params[i].stringIndex = indexArray.at(i);

        int thread_return = pthread_create(&fifoThread[i], NULL, writeFIFO, &params[i]);
        if (thread_return) { return NULL; }
    }

    //Step 4
    for (int i = 0; i < NUMBER_OF_FILES; i++)
    {
        pthread_join(fifoThread[i], NULL);
    }
    printLog("Closed mapping FIFO and threads");

    //Step 5
    printLog("Mapping complete, reduce signaled");
    pthread_mutex_lock(&r_mutex);
    REDUCESIGNAL = true;
    pthread_mutex_unlock(&r_mutex);
    pthread_cond_signal(&r_cond);

    //Step 6
    printLog("## Mapping complete, FIFO's created in 'FIFOFiles' directory");
    return NULL;
}

/*
    1. Set the mapping threads priority using nice
    2. Create an index'd array of vectors based on the GLOBAL array produced at the start of task 3
    3. Sort each of the index arrays based on 3rd character
    4. Create the 13 threads for the FIFO files
    5. Wait for all threads to finish writing to their pipes
    6. Signal reduce to start the merge sort now all the words have been transfered
    7. Return from the mapping thread
*/
void *map4(void *args)
{   
    //Step 1
    nice(-17);

    std::vector<std::vector<int>> indexArray(13, std::vector<int>());
    MapThreadParams params[NUMBER_OF_FILES];
    pthread_t fifoThread[NUMBER_OF_FILES];

    //Step 2
    printLog("Created index'd array");
    int counter = 0;
    for (std::string string : TASK3_GLOBALSTRINGS)  {
        int length = string.length();
        indexArray.at(length - ARRAY_OFFSET).push_back(counter);
        counter++;
    }

    //Step 3
    for(int i = 0; i < NUMBER_OF_FILES; i++) {
        sort(indexArray.at(i).begin(), indexArray.at(i).end(), stringCompareterTask3); 
    }
    

    //Step 4
    printLog("Map Creating " + std::to_string(NUMBER_OF_FILES) + " threads");
    for (int i = 0; i < NUMBER_OF_FILES; i++)
    {
        params[i].index = i; 
        params[i].stringIndex = indexArray.at(i);

        int thread_return = pthread_create(&fifoThread[i], NULL, writeFIFO4, &params[i]);
        if (thread_return) { return NULL; }
    }


    //Step 5
    for (int i = 0; i < NUMBER_OF_FILES; i++)
    {
        pthread_join(fifoThread[i], NULL);
    }
    printLog("Closed mapping FIFO and threads");


    //Step 6
    printLog("Mapping complete, reduce signaled");
    pthread_mutex_lock(&r_mutex);
    REDUCESIGNAL = true;
    pthread_mutex_unlock(&r_mutex);
    pthread_cond_signal(&r_cond);

    //Step 7
    printLog("## Mapping complete, FIFO's created in 'FIFOFiles' directory");
    return NULL;
}