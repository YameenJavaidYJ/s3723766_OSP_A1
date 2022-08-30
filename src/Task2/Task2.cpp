#include <string>
#include <iostream>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include "../../Functions/Commons.h"

const int GRACEFUL_SECONDS = 10;
bool threadExit = false;

std::size_t check_filetype (const std::string& name);
bool check_filename (const std::string& name);
void printLog(std::string print);
void printError(std::string print);
int TaskFilter(const std::string& input, const std::string& output);
int map2(const std::string& input, const std::string& output);
int reduce2(const std::string& output); 

void alarm_handler(int seconds) {
    printError("Program running to long, " + std::to_string(GRACEFUL_SECONDS) + "s set as the limit, exiting");
    threadExit = true; 
}

int main(int argc, char * argv[]) { 
    signal(SIGALRM, alarm_handler); 
    // alarm(GRACEFUL_SECONDS); 

    //Ensure args present
    if(argv[1] == nullptr || argv[2] == nullptr) {
        printError("Invalid usage: './Task2 INPUTFILE.txt OUTPUTFILE.txt'"); 
        return 0; 
    }
    
    std::string input = std::string(argv[1]); 
    std::string output = "Outputs/Task2/" + std::string(argv[2]); 

    //Ensure .txt input and outputs
    if(check_filetype(input) == std::string::npos || check_filetype(output) == std::string::npos) {
        printError("File one of the arguments not .txt format, usage: './Task2 INPUTFILE.txt OUTPUTFILE.txt'");
        return 0;
    }; 

    //Check if the input file is present and accessible, if no print usage
    if(!check_filename(input)) {
        printError("File '" + input + "' not found, usage: './Task2 INPUTFILE.txt OUTPUTFILE.txt'");
        return 0;
    }; 

    printLog("# Using input file: " + input);

    //If TaskFilter returns false, there was an error filtering
    if(!TaskFilter(input, output)) { printError("Error occured while filtering"); return 0; }
    printLog("## Filtering complete, file '" + output + "' created for generic filtered output");

    //If map2 returns false, there was an error exit
    if(!map2(input, output)) { printError("Error occured while mapping "); return 0; }
    printLog("## Mapping complete, filtered files in 'FilteredFiles' directory");

    //Create a _reduced.txt output, if reduce error exit and output
    std::string reducedOutput = output.replace(output.find(".txt"), output.length(), "_reduced.txt"); 
    if(!reduce2(reducedOutput)) { printError("Error occured while reducing"); return 0; }
    printLog("## Reducing complete, single reduced filter files in 'Output/Task2' directory as '" + reducedOutput + "'");

    printLog("# Task 2 Finish, Outputs in 'Outputs/Task2' directory"); 
    return 1;
}