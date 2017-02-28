#ifndef _response_h
#define _response_h

#include "common.h"
#include "headers.h"

typedef struct {
    int status_code;
    char *reason_phrase;
    char *http_ver;
} StatusLine;

typedef struct _response {
    StatusLine *status_line;
    MessageHeader **headers;
    int num_headers;
    char *body;
} Response;

Response *build_response(int status_code, char *body);
void clear_response(Response *response);
void response_struct_to_str(Response *res, char *output_buffer);

#endif
