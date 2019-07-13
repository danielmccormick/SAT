FLAGS = -std=c++14 -Wall -Wextra
FILES = source/formula.* source/sat.cc
COMPILER = g++

lib:
	$(COMPILER) -shared -fpic $(FILES)  -o sat.so $(FLAGS)

bin:
	$(COMPILER) main.cc sat.cc sat.hh -o ./satSolver.o $(FLAGS) -O3

.PHONY: clean
clean: 
	rm -rf *.o *.so
	
