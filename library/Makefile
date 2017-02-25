server: server.o structs.o codes.o
	gcc -pthread server.o structs.o codes.o -o server

structs.o: structs.c structs.h codes.h
	gcc -c structs.c

codes.o: codes.c codes.h
	gcc -c codes.c

server.o: server.c structs.h
	gcc -c server.c

clean:
	rm server server.o structs.o codes.o
