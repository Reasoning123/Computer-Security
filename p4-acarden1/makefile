all: server client

server: server.o
	g++ server.o -o server -lssl -lcrypto

client: client.o
	g++ client.o -o client	-lssl -lcrypto

server.o: server.cpp
	g++ -c server.cpp -lssl -lcrypto

client.o: client.cpp
	g++ -c client.cpp -lssl -lcrypto

clean:
	rm -rf *o server client
	 
