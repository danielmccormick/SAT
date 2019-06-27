FLAGS = -std=c++14 -Wall -Wextra
FILES = source/sat.*

lib:
	g++ -shared -fpic $(FILES)  -o sat.so $(FLAGS)

bin:
	g++ main.cc sat.cc sat.hh -o ./satSolver.o $(FLAGS) -O3

.PHONY: clean
clean: 
	rm -rf *.o *.so
	
