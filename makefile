default: all

#Clean the dir (remove output files)
c:
	rm -f helloworld

#Default action when running make
all:
	g++ -Wall -Werror -std=c++14 -g -O -o  helloworld helloworld.cpp