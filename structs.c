#include <stdio.h>
#include <stdlib.h>
#include <regex.h>

#define MAX_HEADER_SIZE 101

typedef enum {
	HEAD, POST, GET, PUT, DELETE, OPTIONS, CONNECT
} request_type;

typedef struct {
	char * field_name;
	char * field_value; //is field_content
} MessageHeader;

typedef struct {
	request_type req_type;
	char * url;
	char * http_ver;
} RequestLine;

typedef struct {
	int code;
	char * reason_phrase;
} Status;

typedef struct {
	char * http_ver;
	Status status_code;
} StatusLine;

typedef struct {
	StatusLine status_line;
	MessageHeader headers[MAX_HEADER_SIZE];
	char * body;
} Response;

typedef struct {
	RequestLine request_line;
	MessageHeader headers[MAX_HEADER_SIZE];
	char * body;
} Request;

int main() {
	Request req = {{POST, "/api/helloworld", "HTTP/1.1"}};
	req.headers[0] = (MessageHeader) {"Cookie", "chocolate chip"};
	req.headers[1] = (MessageHeader) {"Accept", "*/*"};

	Response res = {{"HTTP/1.1", {200, "OK"}}};
	res.headers[0] = (MessageHeader) {"Content-Type","text/html"};
	res.headers[1] = (MessageHeader) {"Server","dankwebsite.com"};
	res.body = "<h1>Hello World!</h1>";

	return 0;
}