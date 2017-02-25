server: server.o request.o response.o codes.o
	gcc -pthread server.o request.o response.o codes.o -o server

request.o: common.h request.h request.c
	gcc -c request.c

response.o: common.h response.h codes.h response.c
	gcc -c response.c

codes.o: codes.c codes.h
	gcc -c codes.c

server.o: common.h request.h response.h server.c
	gcc -c server.c

clean:
	rm server server.o request.o response.o codes.o
