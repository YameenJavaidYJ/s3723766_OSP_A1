#include <sys/stat.h>
#include <unistd.h>
#include <string>
#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <list>
#include <ranges>

bool check_filename (const std::string& name) {
  struct stat buffer;   
  return (stat (name.c_str(), &buffer) == 0); 
}

bool valid_length(const std::string& string) {
    if(string.length() < 3 || string.length() > 15) {
        return false;
    }

    return true;
}

bool only_lowercase_chars(const std::string& string) {
    return string.find_first_not_of("abcdefghijklmnopqrstuvwxyz") == std::string::npos;
}

bool duplicate_line(const std::string& string, std::string readLines[]) {
    //TODO Fix this, not working
    for(int i = 0; i < *(&readLines + 1) - readLines; i++) {
        if(readLines[i] == string) {
            return true; 
        }
    }

    return false;
}

int number_of_lines(std::ifstream * inputFile) {
    int tmp = 0; 

    for(std::string line; std::getline(*inputFile, line);) {
        tmp++; 
    }

    return tmp;
}


void TaskFilter(const std::string& input, const std::string& output) {
    if(!check_filename(input)) {
        std::cout << "File not found usage: './Task1 INPUTFILE OUTPUTFILE'" << std::endl;
        return;
    }; 

    std::ifstream InputFile(input); 
    std::ofstream CleanFile(output);
    std::string readLines[number_of_lines(&InputFile)];
    
    for(std::string curLine; std::getline(InputFile, curLine);) {
        if(!valid_length(curLine)) { continue; }
        if(!only_lowercase_chars(curLine)) { continue; }
        if(duplicate_line(curLine, readLines)) { continue; }
        //TODO No more than 2 conseq

        CleanFile << curLine << "\n";
    }

    CleanFile.close();
}