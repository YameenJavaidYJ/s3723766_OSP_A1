default: all

#Default action when running make
all:
	g++ -Wall -Werror -std=c++14 -g -O -o  Task1 Task1.cpp

task1: 
	g++ -Wall -Werror -std=c++14 -g -O -o  Task1  TaskFilter.cpp Task1.cpp

task1-c:
	rm -f Task1