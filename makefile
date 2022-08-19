default: all

#Default action when running make
all:
	g++ -Wall -Werror -std=c++14 -g -O -o  Task1  Functions/TaskFilter.cpp Task1.cpp
	g++ -Wall -Werror -std=c++14 -g -O -o  Task2  Functions/Utils.cpp Functions/TaskFilter.cpp  Functions/Map.cpp Functions/Reduce.cpp Task2.cpp

clean:
	rm -f Task1
	rm -f Task2

task1: 
	g++ -Wall -Werror -std=c++14 -O -o  Task1  Functions/Utils.cpp Functions/TaskFilter.cpp Task1.cpp

task1-d: 
	g++ -Wall -Werror -std=c++14 -g -O -o  Task1Debug  Functions/Utils.cpp Functions/TaskFilter.cpp Task1.cpp

task1-c:
	rm -f Task1

task2: 
	g++ -Wall -Werror -std=c++14 -O -o  Task2  Functions/Utils.cpp Functions/TaskFilter.cpp  Functions/Map.cpp Functions/Reduce.cpp Task2.cpp

task2-d: 
	g++ -Wall -Werror -std=c++14 -g -O -o  Task2Debug  Functions/Utils.cpp Functions/TaskFilter.cpp  Functions/Map.cpp Functions/Reduce.cpp Task2.cpp

task2-c:
	rm -f Task2