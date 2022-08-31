#include <vector>
#include <string>

/*
    Global boolean for the main process to signal threads/forks/loops to exit
*/
extern bool THREADEXIT;

/*
    Global array of strings for task3
*/
extern std::vector<std::string> TASK3_GLOBALSTRINGS;

/*
    Global thread signal for reduce
*/
extern bool REDUCESIGNAL;
extern pthread_mutex_t mutex;
extern pthread_cond_t cond;


/*
    Struct used to pass vars through to threads
*/
struct ThreadParams {
    int fifoHandle;
    std::vector<int> stringIndex;
};