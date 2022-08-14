#include <sys/stat.h>
#include <unistd.h>
#include <string>
#include <fstream>
#include <iostream>

bool check_filename (const std::string& name) {
  struct stat buffer;   
  return (stat (name.c_str(), &buffer) == 0); 
}

void TaskFilter(const std::string& input, const std::string& output) {
    if(!check_filename(input)) {
        std::cout << "File not found usage: './Task1 INPUTFILE OUTPUTFILE'" << std::endl;
        return;
    }; 

    std::ofstream CleanFile(output);
}