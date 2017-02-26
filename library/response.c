#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
#include <string.h>
#include "response.h"
#include "codes.h"
#include "date.h"

Response *build_response(int status_code, char *body)
{
    char *reason_phrase = malloc(REASON_BUFFER_SIZE * sizeof(char));
    get_reason_phrase(status_code, reason_phrase);

    StatusLine *status_line = malloc(sizeof(StatusLine));
    *status_line = (StatusLine) { status_code, reason_phrase, HTTP_VERSION };

    MessageHeader **headers = create_headers();

    Response *response = malloc(sizeof(Response));
    *response = (Response) { status_line, headers, 0, body };

    MessageHeader *date = build_header("Date", get_time_stamp());
    add_header_to_response(response, date);

    char length_str[7];
    sprintf(length_str, "%i", strlen(body));
    MessageHeader *length = build_header("Content-Length", length_str);
    add_header_to_response(response, length);

    MessageHeader *connection = build_header("Connection", "Closed");
    add_header_to_response(response, connection);

    return response;
}

void clear_response(Response *response)
{
    free(response->status_line->reason_phrase);
    free(response->status_line);
    clear_headers(response->headers);
    free(response);
}

void response_struct_to_str(Response *res, char *output_buffer)
{
    int i;
    int num_headers = res->num_headers;
    char all_headers_str[BUFFER_SIZE];
    for (i = 0; i < num_headers; i++) {
        MessageHeader *header = res->headers[i];
        char header_str[BUFFER_SIZE];
        sprintf(header_str, "%s: %s\r\n", header->field_name, header->field_value);
        strcat(all_headers_str, header_str);
    }

    sprintf(output_buffer, "%s %i %s\r\n%s\r\n%s\r\n",
                                            res->status_line->http_ver,
                                            res->status_line->status_code,
                                            res->status_line->reason_phrase,
                                            all_headers_str,
                                            res->body);

    printf(output_buffer);
}
