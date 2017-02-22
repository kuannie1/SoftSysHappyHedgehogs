#include <stdio.h>
#include <stdlib.h>

typedef struct http_message {
	request request_line;
	response response_line;
} http_message;

typedef struct generic_message {
	char * startline;
	messageheader message_header;
	crlf CRLF;
	char * message_body;
} message;

typedef struct messageheader {
	char * field_name;
	fieldvalue field_value;
} messageheader;

// I think I still need a struct for field value and everything after that.

typedef struct startline {
	request request_line;
	response response_line;
} startline;

typedef struct request {

} request;

typedef struct response {

} response;
