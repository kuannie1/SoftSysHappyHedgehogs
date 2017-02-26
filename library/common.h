#ifndef _common_h
#define _common_h

#define HTTP_VERSION "HTTP/1.0"
#define BUFFER_SIZE 20000

typedef enum {
    HEAD, POST, GET, PUT, DELETE, OPTIONS, CONNECT
} request_type;

#endif
