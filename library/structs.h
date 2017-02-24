#ifndef _structs_h
#define _structs_h

#define MAX_HEADER_SIZE 101

typedef enum {
	HEAD, POST, GET, PUT, DELETE, OPTIONS, CONNECT
} request_type;

typedef struct {
	char *field_name;
	char *field_value; //is field_content
} MessageHeader;

typedef struct {
	request_type req_type;
	char *url;
	char *http_ver;
} RequestLine;

typedef struct {
	int code;
	char *reason_phrase;
} Status;

typedef struct {
	char *http_ver;
	Status *status;
} StatusLine;

typedef struct {
	StatusLine *status_line;
	MessageHeader headers[MAX_HEADER_SIZE];
	char *body;
} Response;

typedef struct {
	RequestLine *request_line;
	MessageHeader headers[MAX_HEADER_SIZE];
	char *body;
} Request;

Response *build_response(int status_code, char *body);

#endif
