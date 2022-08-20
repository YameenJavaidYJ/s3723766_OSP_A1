#include <sys/stat.h>
#include <unistd.h>
#include <string>
#include <iostream>

/*
    checks a file exists and is accessable
*/
bool check_filename (const std::string& name) {
  struct stat buffer;   
  return (stat (name.c_str(), &buffer) == 0); 
}

/*
    checks a file type for .txt extension
*/
std::size_t check_filetype (const std::string& name) {
  return name.find(".txt");
}

/*
    handles the comparetor sort sorting on the 3rd character
*/
bool stringCompareter(std::string s1, std::string s2) {
    if(s1.length() == 0) { return false; }
    if(s2.length() == 0) { return true; }

    return s1.substr(2, std::string::npos) < s2.substr(2, std::string::npos);
}

void printLog(std::string print) {
  std::cout << std::to_string(getpid()) + " | " + print << std::endl;
}

void printError(std::string print) {
  std::cerr << std::to_string(getpid()) + " | " + print << std::endl;
}