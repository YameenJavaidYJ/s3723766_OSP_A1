#include <sys/stat.h>
#include <unistd.h>
#include <string>
#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <set>

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

    //By using a set to contain the readLines we are ensuring they're no duplicate entries
    std::set<std::string> readLines; 
    
    for(std::string curLine; std::getline(InputFile, curLine);) {
        if(!valid_length(curLine)) { continue; }
        if(!only_lowercase_chars(curLine)) { continue; }
        // //TODO No more than 2 conseq

        readLines.insert(curLine);
    }


    for(std::string string: readLines) {
        CleanFile << string << "\n";
    }

    CleanFile.close();
}