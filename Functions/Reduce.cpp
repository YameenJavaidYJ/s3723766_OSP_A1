#include <string>
#include <fstream>
#include <iostream>


std::ifstream* inputFiles[12]; 

int reduce2(const std::string& output) { 
    for(int i = 0; i <= 12; i++) { 
        std::string inputFile = "FilteredFiles/filter_file_" + std::to_string(i+3) + ".txt";
        inputFiles[i+3] = new std::ifstream(inputFile, std::ofstream::trunc); 
    }

    for(int i = 0; i <= 12; i++) {
        inputFiles[i]->close(); 
    }
}