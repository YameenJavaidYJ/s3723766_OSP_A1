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
    handles the comparetor sort sorting on the 3rd character for task 3 where an index is passed
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

/*
    Used in reduce to move the merge sort pointer and push the recently absorbed string off the queue
*/
std::string getNextLineFromFile(std::queue<std::string>* stringCache) {
    std::string line; 

    if(stringCache->size() == 0) {
        return "";
    }

    //Get the first word in that list and pop it
    line = stringCache->front();
    stringCache->pop(); 

    //Return the first word in the list
    return line; 
}

/*
    Abstract the creation and opening of FIFO files, pass the flag (RD or WR)
    return the FIFO handle
*/
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

/*
    Utility function for the threading of writes in task3
*/        
void *writeFIFO(void *args) {
    struct MapThreadParams *mapData = (struct MapThreadParams *)args;

    //Create/Open FIFO
    int fifoHandle = createOpenFIFO(mapData->index, O_WRONLY); 
    if(fifoHandle == -1) { return NULL; }

    //Write each of the index's for that length to the pipe
    for(int index = 0; index < (int)mapData->stringIndex.size() && !THREADEXIT; index++) {
        if (write(fifoHandle, &mapData->stringIndex.at(index), sizeof(int)) == -1) {
            printError("Error writing to FIFO File");
            return NULL;
        };
    };

    //Close the handle, return the thread
    close(fifoHandle); 
    return NULL;
}

/*
    Utility function for the threading of read in task3
*/   
void *readFIFO(void *args)  {
    struct FIFOThreadParams *mapData = (struct FIFOThreadParams *)args;

    //Create/Open FIFO
    int fifoHandle = createOpenFIFO(mapData->index, O_RDONLY); 
    if(fifoHandle == -1) { return NULL; }

    int index; 
    //Read each of the index's for that length to the pipe and push them to reduce stringCache memory
    while(read(fifoHandle, &index, sizeof(int)) > 0 && !THREADEXIT) {
        mapData->stringCache->push(TASK3_GLOBALSTRINGS.at(index)); 
    }

    //Close the handle, return the thread
    close(fifoHandle); 
    return NULL;
}

/*
    Utility function for the threading of write in task4
*/ 
void *writeFIFO4(void *args) {
    //Set the thread prioirty
    nice(-18);

    struct MapThreadParams *mapData = (struct MapThreadParams *)args;

    //Create/Open FIFO
    int fifoHandle = createOpenFIFO(mapData->index, O_WRONLY); 
    if(fifoHandle == -1) { return NULL; }
    
    //Write each of the index's for that length to the pipe
    for(int index = 0; index < (int)mapData->stringIndex.size() && !THREADEXIT; index++) {
        if (write(fifoHandle, &mapData->stringIndex.at(index), sizeof(int)) == -1) {
            printError("Error writing to FIFO File");
            return NULL;
        };
    };

    close(fifoHandle); 
    return NULL;
}

/*
    Utility function for the threading of read in task4
*/ 
void *readFIFO4(void *args)  {
    //Set the thread prioirty
    nice(-19);

    struct FIFOThreadParams *mapData = (struct FIFOThreadParams *)args;

    //Create/Open FIFO
    int fifoHandle = createOpenFIFO(mapData->index, O_RDONLY); 
    if(fifoHandle == -1) { return NULL; }

    //Read each of the index's for that length to the pipe and push them to reduce stringCache memory
    int index; 
    while(read(fifoHandle, &index, sizeof(int)) > 0 && !THREADEXIT) {
        mapData->stringCache->push(TASK3_GLOBALSTRINGS.at(index)); 
    }

    //Close the handle, return the thread
    close(fifoHandle); 
    return NULL;
}