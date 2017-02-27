#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "request.h"

/* Converts a string to a request_type enum
 */
request_type request_type_string_to_enum(const char *str)
{
    int i;
    for (i=0; i<sizeof(conversion)/sizeof(conversion[0]);i++) {
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
    RequestLine *requestLine = malloc(sizeof(RequestLine));
    char typeBuffer[MAX_REQ_TYPE_SIZE];
    char urlBuffer[LINE_BUFFER_SIZE];
    char versionBuffer[MAX_VERSION_SIZE];

    read_socket_until_stopper(socket, ' ', typeBuffer);
    requestLine->req_type = request_type_string_to_enum(typeBuffer);

    read_socket_until_stopper(socket, ' ', urlBuffer);
    requestLine->url = urlBuffer;

    read_socket_until_stopper(socket, '\r', versionBuffer);
    requestLine->http_ver = versionBuffer;

    return requestLine;
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

    RequestLine *requestLine = build_request_line_from_socket(socket);
    MessageHeader **headers = create_headers();

    *req = (Request) {requestLine, headers, 0, body};

    //Loop through each header line, add to struct as we go
    for (;;) {
        char lineBuffer[LINE_BUFFER_SIZE];
        read_socket_until_stopper(socket, '\r', lineBuffer);

        // If buffer is empty, we're done with headers
        if (!lineBuffer[0]) break;
        add_header_to_request(req, build_header_from_string(lineBuffer));
    }

    return req;
}