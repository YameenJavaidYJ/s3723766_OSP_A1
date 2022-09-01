#include <sys/stat.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>
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
    std::cout << std::to_string(getpid()) + " | " + print << std::endl;
}

/*
    Utility function for quick error logging including the process ID
*/
void printError(std::string print)
{
    std::cerr << std::to_string(getpid()) + " | " + print << std::endl;
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

int countOfStringsLength(int length) {
    int count = 0; 
    for(std::string str: TASK3_GLOBALSTRINGS) {
        if((int)str.length() == length) { count++;}
    }

    return count;
}

void *writeFIFO(void *args)
{
    struct MapThreadParams *mapData = (struct MapThreadParams *)args;

    int numberOfIndexs = mapData->stringIndex.size(); 
    int array[numberOfIndexs]; 

    for(int i = 0; i < numberOfIndexs; i++) {
        array[i] = mapData->stringIndex.at(i); 
    }

    if (write(mapData->fifoHandle, array, sizeof(int) * numberOfIndexs) == -1) {
        printError("Error writing to FIFO File");
        return NULL;
    };

    return NULL;
}
