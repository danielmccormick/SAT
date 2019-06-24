lib:
	g++ -shared -fpic source/sat.hh source/sat.cc -std=c++11 -Wall -Wextra -o sat.so

.PHONY: clean
clean: 
	rm -rf *.o *.so
	
