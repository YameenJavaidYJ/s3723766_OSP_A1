#include <sys/stat.h>
#include <unistd.h>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <string>
#include <set>


bool check_filename (const std::string& name) {
  struct stat buffer;   
  return (stat (name.c_str(), &buffer) == 0); 
}

bool valid_length(const std::string string) {
    if(string.length() < 3 || string.length() > 15)
        return false;
    return true;
}

bool only_lowercase_chars(const std::string string) {
    return string.find_first_not_of("abcdefghijklmnopqrstuvwxyz") == std::string::npos;
}

bool conseq_chars(std::string string) {
    for(int i = 0; i < (int)string.length() - 2; i++)
        if(string[i] == string[i+1] && string[i] == string[i+2])
            return true;
    return false;
}

int TaskFilter(const std::string& input, const std::string& output) {
    //Check if the input file is present and accessible, if no print usage
    if(!check_filename(input)) {
        std::cerr << "File '" << input << "' not found, usage: './Task1 INPUTFILE OUTPUTFILE'" << std::endl;
        return 0;
    }; 

    //Declare the input and clean files
    std::ifstream InputFile(input); 
    std::ofstream CleanFile(output);

    //By using a set to contain the readLines we are ensuring they're no duplicate entries
    std::set<std::string> readLines; 
    
    for(std::string curLine; std::getline(InputFile, curLine);) {
        //If the string is not between 3 and 15 (inclusive), skip
        if(!valid_length(curLine)) { continue; }

        //If the string is not only lowercase chars ([A-Z][0-9] and special chars, skip)
        if(!only_lowercase_chars(curLine)) { continue; }

        //If the string contains more than 2 conseq character, skip
        if(conseq_chars(curLine)) { continue; }

        //Pushing the string to the set handles the uniqueness of each line
        readLines.insert(curLine);
    }

    //Push all the lines that got inserted into the set to the clean file. 
    for(std::string string: readLines) {
        CleanFile << string << "\n";
    }

    InputFile.close();
    CleanFile.close();
    
    return 1;
}