#ifndef _response_h
#define _response_h

#include "common.h"

typedef struct {
    int status_code;
    char *reason_phrase;
    char *http_ver;
} StatusLine;

typedef struct {
    StatusLine *status_line;
    MessageHeader headers[MAX_HEADER_SIZE];
    int num_headers;
    char *body;
} Response;


Response *build_response(int status_code, char *body);
void clear_response(Response *response);

#endif
