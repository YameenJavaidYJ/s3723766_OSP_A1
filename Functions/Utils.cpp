#include <sys/stat.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>
#include <fcntl.h>
#include "Commons.h"

/*
    checks a file exists and is accessable
*/
bool check_filename(const std::string &name)
{
    struct stat buffer;
    return (stat(name.c_str(), &buffer) == 0);
}

/*
    checks a file type for .txt extension
*/
std::size_t check_filetype(const std::string &name)
{
    return name.find(".txt");
}

/*
    handles the comparetor sort sorting on the 3rd character
*/
bool stringCompareter(std::string s1, std::string s2)
{
    if (s1.length() == 0)
    {
        return false;
    }
    if (s2.length() == 0)
    {
        return true;
    }

    return s1.substr(2, std::string::npos) < s2.substr(2, std::string::npos);
}

/*
    handles the comparetor sort sorting on the 3rd character
*/
bool stringCompareterTask3(int i1, int i2)
{
    std::string s1 = TASK3_GLOBALSTRINGS.at(i1);
    std::string s2 = TASK3_GLOBALSTRINGS.at(i2);
    if (s1.length() == 0)
    {
        return false;
    }
    if (s2.length() == 0)
    {
        return true;
    }

    return s1.substr(2, std::string::npos) < s2.substr(2, std::string::npos);
}

/*
    Utility function for quick logging including the process ID
*/
void printLog(std::string print)
{
    std::cout << std::to_string(getpid()) + " | " + print + "\n";
    std::cout.flush();
}

/*
    Utility function for quick error logging including the process ID
*/
void printError(std::string print)
{
    std::cerr << std::to_string(getpid()) + " | " + print + "\n";
    std::cerr.flush();
}

/*
    Utility function to pop the front on an std::vector
*/
void pop_front(std::vector<std::string> &v)
{
    if (v.size() > 0)
    {
        v.erase(v.begin());
    }
}

int createOpenFIFO(int index, int flag) {
    if(mkfifo(("FIFOFiles/fifo_file_" + std::to_string(index + ARRAY_OFFSET)).c_str(), 0777) == -1) {
            if(errno != EEXIST) {
                printError("Could not create FIFO file for index: " + std::to_string(index+ARRAY_OFFSET));
                return -1; 
            }
        }
        
    int handle = open(("FIFOFiles/fifo_file_" + std::to_string(index + ARRAY_OFFSET)).c_str(), flag);
    if(handle == -1) { return -1; }

    return handle; 
}

void *writeFIFO(void *args) {
    struct MapThreadParams *mapData = (struct MapThreadParams *)args;
    int fifoHandle = createOpenFIFO(mapData->index, O_WRONLY); 
    if(fifoHandle == -1) { return NULL; }

    for(int index: mapData->stringIndex) {
        if (write(fifoHandle, &index, sizeof(int)) == -1) {
            printError("Error writing to FIFO File");
            return NULL;
        };
    };

    close(fifoHandle); 
    return NULL;
}

void *readFIFO(void *args)  {
    struct FIFOThreadParams *mapData = (struct FIFOThreadParams *)args;
    int fifoHandle = createOpenFIFO(mapData->index, O_RDONLY); 
    if(fifoHandle == -1) { return NULL; }

    int index; 
    while(read(fifoHandle, &index, sizeof(int)) > 0) {
        mapData->stringCache->push(TASK3_GLOBALSTRINGS.at(index)); 
    }

    close(fifoHandle); 
    return NULL;
}

