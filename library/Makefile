server: server.o request.o response.o headers.o codes.o date.o
	gcc -pthread server.o request.o response.o headers.o codes.o date.o -o server

request.o: common.h request.h headers.h request.c
	gcc -c request.c

response.o: common.h response.h headers.h codes.h date.h response.c
	gcc -c response.c

headers.o: headers.h headers.c
	gcc -c headers.c

codes.o: codes.h codes.c
	gcc -c codes.c

date.o: date.h date.c
	gcc -c date.c

server.o: common.h request.h response.h server.c
	gcc -c server.c

clean:
	rm server server.o request.o response.o codes.o date.o
