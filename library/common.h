#ifndef _common_h
#define _common_h

#define MAX_HEADER_SIZE 101
#define HTTP_VERSION "HTTP/1.0"
#define BUFFER_SIZE 20000

typedef enum {
    HEAD, POST, GET, PUT, DELETE, OPTIONS, CONNECT
} request_type;

typedef struct {
    char *field_name;
    char *field_value; //is field_content
} MessageHeader;

#endif
