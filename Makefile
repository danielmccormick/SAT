FLAGS = -std=c++14 -Wall -Wextra
FILES = source/formula.hh source/formula.cc source/sat.cc
COMPILER = g++

lib:
	$(COMPILER) -shared -fpic $(FILES)  -o sat.so $(FLAGS)

bin:
	$(COMPILER) $(FILES) source/main.cc -o ./satSolver.o $(FLAGS) -O3

.PHONY: clean
clean: 
	rm -rf *.o *.so
	
