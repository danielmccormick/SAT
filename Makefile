lib:
	g++ -shared -fpic source/proplogic.cc -o libprop.so -std=c++11 -Wfatal-errors

.PHONY: clean
clean: 
	rm -rf *.o *.so
	
