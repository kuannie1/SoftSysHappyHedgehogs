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

void clear_headers(MessageHeader **headers)
{
    size_t i;
    for (i = 0; i < MAX_HEADER_SIZE; i++) {
        free(headers[i]);
    }
    free(headers);
}

void add_header(Response *response, MessageHeader *header)
{
    response->headers[response->num_headers] = header;
    response->num_headers++;
}
