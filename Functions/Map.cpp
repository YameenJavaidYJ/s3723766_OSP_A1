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

const int ARRAY_SIZE = 13;
const int ARRAY_OFFSET = 3;

bool stringCompareter(std::string s1, std::string s2);
void printLog(std::string print);
void printError(std::string print);
void *writeFIFO(void *args);

int map2(const std::string &input, const std::string &output)
{
    std::ofstream outputFiles[ARRAY_SIZE];
    std::vector<std::string> inputStreams[ARRAY_SIZE];
    std::ifstream InputFile;

    printLog("Creating the individual output files");

    for (int i = 0; i < ARRAY_SIZE && !THREADEXIT; i++)
    {
        std::string outputFile = "FilteredFiles/filtered_file_" + std::to_string(i + ARRAY_OFFSET) + ".txt";
        outputFiles[i].open(outputFile, std::ofstream::trunc);
        printLog("filter_file_" + std::to_string(i + ARRAY_OFFSET) + " created");
    }

    InputFile.open(output);

    for (std::string curLine; std::getline(InputFile, curLine);)
    {
        inputStreams[curLine.length() - ARRAY_OFFSET].push_back(curLine);
    }

    for (int i = 0; i < ARRAY_SIZE && !THREADEXIT; i++)
    {
        if (fork() == 0)
        {
            printLog("Sorting strings of length " + std::to_string(i + ARRAY_OFFSET));
            sort(inputStreams[i].begin(), inputStreams[i].end(), stringCompareter);
            printLog("Sorting complete for length " + std::to_string(i + ARRAY_OFFSET));
            for (std::string str : inputStreams[i])
            {
                outputFiles[i] << str << "\n";
            }
            printLog("Written to filter_file_" + std::to_string(i + ARRAY_OFFSET) + ".txt");
            exit(0);
        }
    }

    while (wait(NULL) != -1 || errno != ECHILD)
        ;

    for (int i = 0; i < ARRAY_SIZE; i++)
    {
        outputFiles[i].close();
    }

    InputFile.close();

    return 1;
}

void *map3(void *args)
{
    std::vector<std::vector<int>> indexArray(13, std::vector<int>());
    int fifoHandles[ARRAY_SIZE];
    ThreadParams params[ARRAY_SIZE];
    pthread_t fifoThread[ARRAY_SIZE];

    printLog("Created index'd array");
    int counter = 0;
    for (std::string string : TASK3_GLOBALSTRINGS)
    {
        int length = string.length();
        indexArray.at(length - ARRAY_OFFSET).push_back(counter);
        counter++;
    }

    printLog("Creating/Openning map FIFO Files");

    for (int i = 0; i < ARRAY_SIZE; i++)
    {
        if (mkfifo(("FIFOFiles/fifo_file_" + std::to_string(i + ARRAY_OFFSET)).c_str(), 0777) == -1)
        {
            if (errno != EEXIST)
            {
                printError("Could not create FIFO file for index: " + std::to_string(i + ARRAY_OFFSET));
                return NULL;
            }
        }

        int handle = open(("FIFOFiles/fifo_file_" + std::to_string(i + ARRAY_OFFSET)).c_str(), O_WRONLY);
        fifoHandles[i] = handle;

        params[i].fifoHandle = handle;
        params[i].stringIndex = indexArray.at(i);

        printLog("Creating thread " + std::to_string(i));
        int thread_return = pthread_create(&fifoThread[i], NULL, writeFIFO, &params[i]);
        if (thread_return)
        {
            return NULL;
        }
    }

    printLog("Closing map FIFO handles");
    for (int i = 0; i < ARRAY_SIZE; i++)
    {
        close(fifoHandles[i]);
        pthread_join(fifoThread[i], NULL);
    }

    pthread_mutex_lock(&mutex);
    REDUCESIGNAL = true;
    pthread_mutex_unlock(&mutex);
    pthread_cond_signal(&cond);

    printLog("## Mapping complete, FIFO's created in 'FIFOFiles' directory, reduce thread has been signaled");

    return NULL;
}