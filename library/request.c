#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "request.h"

/* Converts a string to a request_type enum
 */
request_type request_type_string_to_enum(const char *str)
{
    int i;
    for (i = 0; i < sizeof(conversion)/sizeof(conversion[0]); i++) {
        if (!strcmp(str, conversion[i].str)) {
            return conversion[i].val;
        }
    }
    printf("ERROR: No such request type: %s\n", str);
}

/* Reads the socket char-by-char to the buffer until it hits the stopper
 * character
 */
void read_socket_until_stopper(int socket, char stopper, char *buffer) {
    int i;
    for (i = 0; i<LINE_BUFFER_SIZE; i++) {
        char char_buffer;
        recv(socket, &char_buffer, 1, 0);
        if (char_buffer == stopper || char_buffer == '\r') {
            buffer[i] = '\0';
            if (char_buffer == '\r') {
                // Read the next \n too to get rid of it
                recv(socket, &char_buffer, 1, 0);
            }
            break;
        }
        buffer[i] = char_buffer;
    }
}

/* Assumes that the socket is currently at the request line. Builds a RequestLine
 * struct by reading from the socket word-by-word.
 */
RequestLine *build_request_line_from_socket(int socket)
{
    RequestLine *request_line = malloc(sizeof(RequestLine));
    char type_buffer[MAX_REQ_TYPE_SIZE];
    char *url_buffer = malloc(LINE_BUFFER_SIZE * sizeof(char));
    char *version_buffer = malloc(MAX_VERSION_SIZE * sizeof(char));

    read_socket_until_stopper(socket, ' ', type_buffer);
    request_line->req_type = request_type_string_to_enum(type_buffer);

    read_socket_until_stopper(socket, ' ', url_buffer);
    request_line->url = url_buffer;

    read_socket_until_stopper(socket, '\r', version_buffer);
    request_line->http_ver = version_buffer;

    return request_line;
}

/* Builds and returns a MessageHeader struct from the source string.
 *
 * str: source string in the format "name: value"
 */
MessageHeader *build_header_from_string(char *str)
{
    char *copy = malloc(sizeof(char) * strlen(str));
    char *name;
    char *val;

    strcpy(copy, str);
    name = strtok(copy, ":");
    val = strtok(NULL, "");

    // Get rid of leading space
    val = val + 1;
    return build_header(name, val);
}

/* Builds and returns a Request struct from reading a request from the socket
 */
Request *build_request_from_socket(int socket)
{
    Request *req = malloc(sizeof(Request));
    char *body;

    RequestLine *request_line = build_request_line_from_socket(socket);
    MessageHeader **headers = create_headers();

    *req = (Request) { request_line, headers, 0, body };

    //Loop through each header line, add to struct as we go
    for (;;) {
        char line_buffer[LINE_BUFFER_SIZE];
        read_socket_until_stopper(socket, '\r', line_buffer);

        // If buffer is empty, we're done with headers
        if (!line_buffer[0]) break;
        add_header_to_request(req, build_header_from_string(line_buffer));
    }

    return req;
}

void clear_request(Request *request)
{
    free(request->request_line->url);
    free(request->request_line->http_ver);
    free(request->request_line);
    clear_headers(request->headers, request->num_headers);
    free(request->body);
    free(request);
}
