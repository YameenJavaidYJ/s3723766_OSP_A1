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

bool stringCompareter(std::string s1, std::string s2);
bool stringCompareterTask3(int i1, int i2);
void printLog(std::string print);
void printError(std::string print);
void *writeFIFO(void *args);

int map2(const std::string &input, const std::string &output)
{
    std::vector<std::string> inputStreams[NUMBER_OF_FILES];
    std::ifstream InputFile;

    InputFile.open(output);

    for (std::string curLine; std::getline(InputFile, curLine);)
    {
        inputStreams[curLine.length() - ARRAY_OFFSET].push_back(curLine);
    }

    InputFile.close();

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
            OutputFile.close();
            printLog("filter_file_" + std::to_string(i + ARRAY_OFFSET) + " closed, fork finished");
            exit(0);
        }
    }

    while (wait(NULL) != -1 || errno != ECHILD);

    printLog("Mapping forks finished");

    return 1;
}

void *map3(void *args)
{
    std::vector<std::vector<int>> indexArray(13, std::vector<int>());
    MapThreadParams params[NUMBER_OF_FILES];
    pthread_t fifoThread[NUMBER_OF_FILES];

    printLog("Created index'd array");
    int counter = 0;
    for (std::string string : TASK3_GLOBALSTRINGS)  {
        int length = string.length();
        indexArray.at(length - ARRAY_OFFSET).push_back(counter);
        counter++;
    }

    for(std::vector<int> ve: indexArray) {
        printError(std::to_string(ve.size()));
    }


    for(int i = 0; i < NUMBER_OF_FILES; i++) {
        sort(indexArray.at(i).begin(), indexArray.at(i).end(), stringCompareterTask3); 
    }
    
    printLog("Map Creating " + std::to_string(NUMBER_OF_FILES) + " threads");
    for (int i = 0; i < NUMBER_OF_FILES; i++)
    {
        params[i].index = i; 
        params[i].stringIndex = indexArray.at(i);

        int thread_return = pthread_create(&fifoThread[i], NULL, writeFIFO, &params[i]);
        if (thread_return) { return NULL; }
    }

    for (int i = 0; i < NUMBER_OF_FILES; i++)
    {
        pthread_join(fifoThread[i], NULL);
    }
    printLog("Closed mapping FIFO and threads");

    printLog("Mapping complete, reduce signaled");
    pthread_mutex_lock(&r_mutex);
    REDUCESIGNAL = true;
    pthread_mutex_unlock(&r_mutex);
    pthread_cond_signal(&r_cond);

    printLog("## Mapping complete, FIFO's created in 'FIFOFiles' directory");

    return NULL;
}