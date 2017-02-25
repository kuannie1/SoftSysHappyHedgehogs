#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
#include "structs.h"
#include "codes.h"

#define HTTP_VERSION "HTTP/1.0"

Response *build_response(int status_code, char *body)
{
    char reason_phrase[REASON_BUFFER_SIZE];
    get_reason_phrase(status_code, reason_phrase);

    Status *status = malloc(sizeof(Status));
    *status = (Status) { status_code, reason_phrase };

    StatusLine *status_line = malloc(sizeof(StatusLine));
    *status_line = (StatusLine) { HTTP_VERSION, status };

    Response *response = malloc(sizeof(Response));
    *response = (Response) { status_line, {}, 0, body };

    return response;
}

void clear_response(Response *response)
{
    free(response->status_line->http_ver);
    free(response->status_line->status->reason_phrase);
    free(response->status_line->status);
    free(response->status_line);
    free(response->body);
}
