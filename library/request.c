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
        if (char_buffer == stopper) {
            buffer[i] = '\0';
            if (stopper == '\r') {
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

/* Calls recv() char-by-char until it hits a \r\n, then puts that string
 * into the buffer.
 */
void get_line_from_socket(int socket, char *buffer)
{
    read_socket_until_stopper(socket, '\r', buffer);
}

/* Assume the socket is currently at a header line. Builds and returns a
 * MessageHeader struct with the name and value of the current line.
 */
MessageHeader *build_header_from_socket(int socket)
{
    int i;
    char nameBuffer[LINE_BUFFER_SIZE];
    char valBuffer[LINE_BUFFER_SIZE];

    read_socket_until_stopper(socket, ':', nameBuffer);
    read_socket_until_stopper(socket, '\r', valBuffer);
}

/* Builds and returns a Request struct from reading a request from the socket
 */
Request *build_request_from_socket(int socket)
{
    Request *req = malloc(sizeof(Request));
    char lineBuffer[LINE_BUFFER_SIZE];

    // Get the first request line
    RequestLine *requestLine = build_request_line_from_socket(socket);
    req->request_line = requestLine;

    //Loop through each header line, add to struct as we go

    return req;
}