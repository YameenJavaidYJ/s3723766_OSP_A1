default: all

#Default action when running make
all:
	g++ -Wall -Werror -std=c++14 -O -o  Executables/Task1 Functions/Commons.h Functions/Utils.cpp Functions/TaskFilter.cpp src/Task1/Task1.cpp
	g++ -Wall -Werror -std=c++14 -O -o  Executables/Task2 Functions/Commons.h Functions/Utils.cpp Functions/TaskFilter.cpp Functions/Map.cpp Functions/Reduce.cpp src/Task2/Task2.cpp -pthread
	g++ -Wall -Werror -std=c++14 -O -o  Executables/Task3 Functions/Commons.h Functions/Utils.cpp Functions/TaskFilter.cpp Functions/Map.cpp Functions/Reduce.cpp src/Task3/Task3.cpp -pthread
	g++ -Wall -Werror -std=c++14 -O -o  Executables/Task4 Functions/Commons.h Functions/Utils.cpp Functions/TaskFilter.cpp Functions/Map.cpp Functions/Reduce.cpp src/Task3/Task4.cpp -pthread

clean:
	rm -f Executables/Task1
	rm -f Executables/Debug/Task1Debug
	rm -f Executables/Task2
	rm -f Executables/Debug/Task2Debug
	rm -f Executables/Task3
	rm -f Executables/Debug/Task3Debug
	rm -f Executables/Task4
	rm -f Executables/Debug/Task4Debug

task1: 
	g++ -Wall -Werror -std=c++14 -O -o  Executables/Task1 Functions/Commons.h Functions/Utils.cpp Functions/TaskFilter.cpp src/Task1/Task1.cpp

task1-d: 
	g++ -Wall -Werror -std=c++14 -g -O -o  Executables/Debug/Task1Debug Functions/Commons.h Functions/Utils.cpp Functions/TaskFilter.cpp src/Task1/Task1.cpp

task1-c:
	rm -f Executables/Task1
	rm -f Executables/Debug/Task1Debug

task2: 
	g++ -Wall -Werror -std=c++14 -O -o  Executables/Task2 Functions/Commons.h Functions/Utils.cpp Functions/TaskFilter.cpp  Functions/Map.cpp Functions/Reduce.cpp src/Task2/Task2.cpp -pthread

task2-d: 
	g++ -Wall -Werror -std=c++14 -g -O -o  Executables/Debug/Task2Debug Functions/Commons.h Functions/Utils.cpp Functions/TaskFilter.cpp  Functions/Map.cpp Functions/Reduce.cpp src/Task2/Task2.cpp -pthread

task2-c:
	rm -f Executables/Task2
	rm -f Executables/Debug/Task2Debug

task3: 
	g++ -Wall -Werror -std=c++14 -O -o  Executables/Task3 Functions/Commons.h Functions/Utils.cpp Functions/TaskFilter.cpp  Functions/Map.cpp Functions/Reduce.cpp src/Task3/Task3.cpp -pthread

task3-d: 
	g++ -Wall -Werror -std=c++14 -g -O -o  Executables/Debug/Task3Debug Functions/Commons.h Functions/Utils.cpp Functions/TaskFilter.cpp  Functions/Map.cpp Functions/Reduce.cpp src/Task3/Task3.cpp -pthread

task3-c:
	rm -f Executables/Task3
	rm -f Executables/Debug/Task3Debug

task4: 
	g++ -Wall -Werror -std=c++14 -O -o  Executables/Task4 Functions/Commons.h Functions/Utils.cpp Functions/TaskFilter.cpp  Functions/Map.cpp Functions/Reduce.cpp src/Task4/Task4.cpp -pthread

task4-d: 
	g++ -Wall -Werror -std=c++14 -g -O -o  Executables/Debug/Task4Debug Functions/Commons.h Functions/Utils.cpp Functions/TaskFilter.cpp  Functions/Map.cpp Functions/Reduce.cpp src/Task4/Task4.cpp -pthread

task4-c:
	r -f Executables/Task4
	rm -f Executables/Debug/Task4Debug