#ifndef _request_h
#define _request_h

#include "common.h"
#include "headers.h"

typedef struct {
    request_type req_type;
    char *url;
    char *http_ver;
} RequestLine;

typedef struct _request {
    RequestLine *request_line;
    MessageHeader **headers;
    int num_headers;
    char *body;
} Request;

Request *build_request_from_socket(int socket);

#endif
