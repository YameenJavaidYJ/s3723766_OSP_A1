default: all

#Default action when running make
all:
	g++ -Wall -Werror -std=c++14 -g -O -o  Executables/Task1  Functions/Utils.cpp Functions/TaskFilter.cpp src/Task1/Task1.cpp
	g++ -Wall -Werror -std=c++14 -g -O -o  Executables/Task2 Functions/Commons.h Functions/Utils.cpp Functions/TaskFilter.cpp Functions/Map.cpp Functions/Reduce.cpp src/Task2/Task2.cpp

clean:
	rm -f Executables/Task1
	rm -f Executables/Debug/Task1Debug
	rm -f Executables/Task2
	rm -f Executables/Debug/Task2Debug

task1: 
	g++ -Wall -Werror -std=c++14 -O -o  Executables/Task1 Functions/Commons.h Functions/Utils.cpp Functions/TaskFilter.cpp src/Task1/Task1.cpp

task1-d: 
	g++ -Wall -Werror -std=c++14 -g -O -o  Executables/Debug/Task1Debug Functions/Commons.h Functions/Utils.cpp Functions/TaskFilter.cpp src/Task1/Task1.cpp

task1-c:
	rm -f Executables/Task1
	rm -f Executables/Debug/Task1Debug

task2: 
	g++ -Wall -Werror -std=c++14 -O -o  Executables/Task2 Functions/Commons.h Functions/Utils.cpp Functions/TaskFilter.cpp  Functions/Map.cpp Functions/Reduce.cpp src/Task2/Task2.cpp

task2-d: 
	g++ -Wall -Werror -std=c++14 -g -O -o  Executables/Debug/Task2Debug Functions/Commons.h Functions/Utils.cpp Functions/TaskFilter.cpp  Functions/Map.cpp Functions/Reduce.cpp src/Task2/Task2.cpp

task2-c:
	rm -f Executables/Task2
	rm -f Executables/Debug/Task2Debug