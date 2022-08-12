default: all

clean:
	rm -f helloworld

debug:
	g++ -Wall -Werror -std=c++14 -g -O -o  helloworld helloworld.cpp

# all: 
# 	g++ -Wall -Werror -std=c++14 -O -o  question1 question1.cpp