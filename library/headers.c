#include <stdlib.h>
#include "headers.h"
#include "request.h"
#include "response.h"

MessageHeader **create_headers()
{
    MessageHeader **headers = malloc(MAX_HEADER_SIZE * sizeof(MessageHeader *));
    return headers;
}

MessageHeader *build_header(char *field, char *value)
{
    MessageHeader *header = malloc(sizeof(MessageHeader));
    *header = (MessageHeader) { strdup(field), strdup(value) };
    return header;
}

void clear_headers(MessageHeader **headers, int num_headers)
{
    size_t i;
    for (i = 0; i < num_headers; i++) {
        free(headers[i]->field_name);
        free(headers[i]->field_value);
        free(headers[i]);
    }
    free(headers);
}

void print_headers(MessageHeader **headers, int num_headers)
{
    int i;
    for (i = 0; i < num_headers; i++) {
        printf("%s: %s\r\n", headers[i]->field_name, headers[i]->field_value);
    }
    printf("\r\n");
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
