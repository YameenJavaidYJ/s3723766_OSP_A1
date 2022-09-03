#include <iostream>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include "../../Functions/Commons.h"

const int GRACEFUL_SECONDS = 10;
bool THREADEXIT = false;

/*
    NOTE These are included so the Reduce.cpp holding task3 compiles here. 
    Not used
*/
bool REDUCESIGNAL = false;
pthread_mutex_t r_mutex;
pthread_cond_t r_cond;
std::vector<std::string> TASK3_GLOBALSTRINGS; 

void printLog(std::string print);
void printError(std::string print);
std::size_t check_filetype (const std::string& name);
bool check_filename (const std::string& name);
int TaskFilter(const std::string& input, const std::string& output);

void alarm_handler(int seconds) {
    std::cerr << "Program running to long, " << GRACEFUL_SECONDS << "s set as the limit, exiting" << std::endl;
    THREADEXIT = true; 
}

int main(int argc, char * argv[]) { 
    signal(SIGALRM, alarm_handler); 
    alarm(GRACEFUL_SECONDS); 
    
    if(argv[1] == nullptr || argv[2] == nullptr) {
        printError("Invalid usage: './Task2 INPUTFILE.txt OUTPUTFILE.txt'"); 
        return EXIT_FAILURE; 
    }
    
    std::string input = std::string(argv[1]); 
    std::string output = "Outputs/Task1/" + std::string(argv[2]); 

    //Ensure .txt input and outputs
    if(check_filetype(input) == std::string::npos || check_filetype(output) == std::string::npos) {
        printError("File one of the arguments not .txt format, usage: './Task2 INPUTFILE.txt OUTPUTFILE.txt'");
        return EXIT_FAILURE;
    }; 

    //Check if the input file is present and accessible, if no print usage
    if(!check_filename(input)) {
        printError("File '" + input + "' not found, usage: './Task2 INPUTFILE.txt OUTPUTFILE.txt'");
        return EXIT_FAILURE;
    }; 

    printLog("Using input file: " + input);

    //If TaskFilter returns false, there was an error filtering
    if(!TaskFilter(input, output)) { printError("Error occured while filtering");  return EXIT_FAILURE; }
    printLog("Filtering complete, file '" + output + "' created in 'Outputs/Task1'");

    return EXIT_SUCCESS;
}