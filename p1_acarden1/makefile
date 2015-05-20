
all:rc6

rc6: secure.o 
	g++ -g -o rc6 secure.o 

secure.o: secure.cpp
	g++ -c -g secure.cpp

clean: 
	-rm rc6 secure.o 
