#ifndef _common_h
#define _common_h

#define HTTP_VERSION "HTTP/1.0"
#define BUFFER_SIZE 20000
#define LINE_BUFFER_SIZE 1024
#define MAX_REQ_TYPE_SIZE 7
#define MAX_VERSION_SIZE 8

typedef enum {
    HEAD, POST, GET, PUT, DELETE, OPTIONS, CONNECT
} request_type;

const static struct {
    request_type val;
    const char *str;
} conversion [] = {
    {HEAD, "HEAD"},
    {POST, "POST"},
    {GET, "GET"},
    {PUT, "PUT"},
    {DELETE, "DELETE"},
    {OPTIONS, "OPTIONS"},
    {CONNECT, "CONNECT"}
};

typedef struct {
    char *field_name;
    char *field_value; //is field_content
} MessageHeader;

#endif
