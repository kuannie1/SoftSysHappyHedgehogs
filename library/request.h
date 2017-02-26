#ifndef _request_h
#define _request_h

#include "common.h"

typedef struct {
    request_type req_type;
    char *url;
    char *http_ver;
} RequestLine;

typedef struct {
    RequestLine *request_line;
    MessageHeader headers[MAX_HEADER_SIZE];
    char *body;
} Request;

#endif