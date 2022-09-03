#include <string>
#include <pthread.h>
#include <iostream>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <fstream>
#include "../../Functions/Commons.h"

const int GRACEFUL_SECONDS = 10;
bool THREADEXIT = false;
bool REDUCESIGNAL = false;
pthread_mutex_t r_mutex;
pthread_cond_t r_cond;
std::vector<std::string> TASK3_GLOBALSTRINGS; 

std::size_t check_filetype (const std::string& name);
bool check_filename (const std::string& name);
void printLog(std::string print);
void printError(std::string print);
int TaskFilter(const std::string& input, const std::string& output);
void* map4(void* args);
void* reduce4(void* args); 

void alarm_handler(int seconds) {
    printError("Program running to long, " + std::to_string(GRACEFUL_SECONDS) + "s set as the limit, exiting");
    THREADEXIT = true; 
}

int main(int argc, char * argv[]) { 
    signal(SIGALRM, alarm_handler); 
    alarm(GRACEFUL_SECONDS); 

    //Ensure args present
    if(argv[1] == nullptr || argv[2] == nullptr) {
        printError("Invalid usage: './Task4 INPUTFILE.txt OUTPUTFILE.txt'"); 
        return EXIT_FAILURE; 
    }
    
    std::string input = std::string(argv[1]); 
    std::string output = "Outputs/Task4/" + std::string(argv[2]); 

    //Ensure .txt input and outputs
    if(check_filetype(input) == std::string::npos || check_filetype(output) == std::string::npos) {
        printError("File one of the arguments not .txt format, usage: './Task4 INPUTFILE.txt OUTPUTFILE.txt'");
        return EXIT_FAILURE;
    }; 

    //Check if the input file is present and accessible, if no print usage
    if(!check_filename(input)) {
        printError("File '" + input + "' not found, usage: './Task4 INPUTFILE.txt OUTPUTFILE.txt'");
        return EXIT_FAILURE;
    }; 
    
    printLog("# Using input file: " + input);

    //If TaskFilter returns false, there was an error filtering
    if(!TaskFilter(input, output)) { printError("Error occured while filtering"); return 0; }
    printLog("## Filtering complete, file '" + output + "' created for generic filtered output");

    //Read the filtered file into global array memory
    std::ifstream InputFile; 
    InputFile.open(output); 
    
    for(std::string curLine; std::getline(InputFile, curLine);) {
        TASK3_GLOBALSTRINGS.push_back(curLine); 
    }

    //Close the input file stream
    InputFile.close(); 

    //Create/Init thread properties, mutex and cond used for mapping to signal reduce
    pthread_t mappingThread; 
    pthread_t reducingThread; 
    pthread_mutex_init(&r_mutex, NULL);
    pthread_cond_init(&r_cond, NULL);

    //Create the mapping thread, return if error
    int thread_map_return = pthread_create(&mappingThread, NULL, map4, NULL); 
    if (thread_map_return) { return EXIT_FAILURE; }

    //Define the output name for reduce, append '_reduce.txt'
    ReduceThreadParams params; 
    params.output = output.replace(output.find(".txt"), output.length(), "_reduced.txt"); 
    
    //Create the reduce thread, return if error
    int thread_reduce_return = pthread_create(&reducingThread, NULL, reduce4, &params); 
    if (thread_reduce_return) { return EXIT_FAILURE; }

    //Process waits and joins when the threads have returned
    pthread_join(reducingThread, NULL); 
    pthread_join(mappingThread, NULL); 

    //Destory the reduce signalling mutex and cond
    pthread_mutex_destroy(&r_mutex);
    pthread_cond_destroy(&r_cond);

    //Task4 Complete
    printLog("# Task 3 Finish, Outputs in 'Outputs/Task4' directory"); 
    return EXIT_SUCCESS;
}