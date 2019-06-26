FLAGS = -std=c++14 -Wall -Wextra
FILES = source/sat.*

lib:
	g++ -shared -fpic $(FILES)  -o sat.so $(FLAGS)

.PHONY: clean
clean: 
	rm -rf *.o *.so
	
