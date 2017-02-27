VPATH = ./library

example: example.o server.o request.o response.o headers.o codes.o date.o
	gcc -pthread example.o server.o request.o response.o headers.o codes.o date.o -o example

request.o: common.h request.h headers.h request.c
	gcc -c library/request.c

response.o: common.h response.h headers.h codes.h date.h response.c
	gcc -c library/response.c

headers.o: headers.h headers.c
	gcc -c library/headers.c

codes.o: codes.h codes.c
	gcc -c library/codes.c

date.o: date.h date.c
	gcc -c library/date.c

server.o: common.h request.h response.h server.h server.c
	gcc -c library/server.c

example.o: common.h request.h response.h headers.h codes.h date.h server.h example.c
	gcc -c example.c

clean:
	rm example example.o server.o request.o response.o headers.o codes.o date.o
