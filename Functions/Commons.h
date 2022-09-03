#include <vector>
#include <string>
#include <queue>

const int NUMBER_OF_FILES = 13;
const int ARRAY_OFFSET = 3;

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
extern pthread_mutex_t r_mutex;
extern pthread_cond_t r_cond;


/*
    Struct used to pass vars through to threads
*/
struct MapThreadParams {
    int index;
    std::vector<int> stringIndex;
};

/*
    Struct used to pass vars through to reduce thread
*/
struct ReduceThreadParams {
    std::string output; 
};

/*
    Struct used to pass vars through to reduce FIFO threads
*/
struct FIFOThreadParams {
    int index;
    std::queue<std::string>* stringCache; 
};