#include <stdio.h>
#include <stdlib.h>

typedef enum {
	HEAD, POST, GET, PUT, DELETE, OPTIONS, CONNECT
} request_type;

typedef struct messageheader {
	char * field_name;
	char * field_value; //is field_content
} messageheader;

typedef struct req_startline {
	request_type req_type;
	char * url;
	char * http_ver;
} req_startline;

typedef struct status {
	int code;
	char * text;
} status;

typedef struct res_startline {
	char * http_ver;
	status status_code;
} res_startline;


typedef struct response {
	res_startline startline;
	messageheader headers[101];
} response;

typedef struct request {
	req_startline startline;
	messageheader headers[101];
} request;

int main() {
	request req = {{GET, "/api/testing", "HTTP/1.1"}};
	req.headers[0] = (messageheader) {"Cookie", "chocolate chip"};
	req.headers[1] = (messageheader) {"Accept", "*/*"};

	response res = {{"HTTP/1.1", {200, "OK"}}};
	res.headers[0] = (messageheader) {"Content-Type","text/html"};
	res.headers[1] = (messageheader) {"Server","dankwebsite.com"};
	return 0;
}