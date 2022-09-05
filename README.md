# Assessment 1 - OSP - 40% By Jordan Edwin Hacking s3723766
## Usage
To compile the C++ file you can run `make` (defaults to making all executables) or you can run `make task1|task2|task4|task4`

All executable outputs are inside of the `Executables` directory.

Running `make clean` will remove all traces of the executables.

### Arguments
Each executable tasks the same arguements 

1. Word list to clean and sort
2. Output file name
3. 1>outlog file name (optional)
4. 2>errlog file name (optional)

**Arguments 1 and 2 MUST be .txt** file formats or the executable will display an error asking for a .txt file

### Structure
For each executable to run correctly ensure the following folders have been created inside the directory where the execution will occur, the submission .zip file will have this directory already created.

- Outputs
    - Task1
    - Task2
    - Task3
    - Task4
- FilteredFiles
- FIFOFiles
- Wordlists

Failure to have these directories created may lead to issues executing as the output paths have beed hard-coded to each tasks output folder, as well as .txt and .fifo file creation locations

### Outputs
The outputs folder for task 2,3 and 4 will have two different .txt files, one which is the arguement passed in this one represents the raw unsorted Task1Filter output and the second file will have _reduced.txt appeneded to the end, this will be the reduced output sorted on the 3rd character. 

