#include <stdlib.h>
#include "headers.h"
#include "response.h"

MessageHeader **create_headers()
{
    MessageHeader **headers = malloc(MAX_HEADER_SIZE * sizeof(MessageHeader *));

    size_t i;
    for (i = 0; i < MAX_HEADER_SIZE; i++) {
        headers[i] = malloc(sizeof(MessageHeader));
    }

    return headers;
}

MessageHeader *build_header(char *field, char *value)
{
    MessageHeader *header = malloc(sizeof(MessageHeader));
    *header = (MessageHeader) { field, value };
    return header;
}

void clear_headers(MessageHeader **headers)
{
    size_t i;
    for (i = 0; i < MAX_HEADER_SIZE; i++) {
        free(headers[i]);
    }
    free(headers);
}

void add_header_to_request(Request *request, MessageHeader *header)
{
    request->headers[request->num_headers] = header;
    request->num_headers++;
}

void add_header_to_response(Response *response, MessageHeader *header)
{
    response->headers[response->num_headers] = header;
    response->num_headers++;
}
