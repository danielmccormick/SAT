FLAGS = -std=c++14 -Wall -Wextra
FILES = source/formula.hh source/formula.cc source/sat.cc
COMPILER = g++

lib:
	$(COMPILER) -shared -fpic source/sat.cc -o sat.so $(FLAGS)

bin: lib
	$(COMPILER) source/main.cc -o ./satSolver.o $(FLAGS) -O3

test: bin test.sh
	./test.sh

testSmall: bin 
	./satSolver.o test/sample0.cnf

.PHONY: clean
clean: 
	rm -rf *.o *.so
	
