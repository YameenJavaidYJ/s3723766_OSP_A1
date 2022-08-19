#include <sys/stat.h>
#include <unistd.h>
#include <string>

/*
    checks a file exists and is accessable
*/
bool check_filename (const std::string& name) {
  struct stat buffer;   
  return (stat (name.c_str(), &buffer) == 0); 
}