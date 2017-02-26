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

/* Calls recv() char-by-char until it hits a space or a \r, then puts that
 * string into the buffer.
 */
void get_word_from_socket(int socket, char *buffer)
{
    int i;
    for (i = 0; i<LINE_BUFFER_SIZE; i++) {
        char char_buffer;
        recv(socket, &char_buffer, 1, 0);
        if (char_buffer == ' ') {
            buffer[i] = '\0';
            break;
        } else if (char_buffer == '\r') {
            buffer[i] = '\0';
            // Read the next \n too to get rid of it
            recv(socket, &char_buffer, 1, 0);
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

    get_word_from_socket(socket, typeBuffer);
    requestLine->req_type = request_type_string_to_enum(typeBuffer);

    get_word_from_socket(socket, urlBuffer);
    requestLine->url = urlBuffer;

    get_word_from_socket(socket, versionBuffer);
    requestLine->http_ver = versionBuffer;

    return requestLine;
}

/* Calls recv() char-by-char until it hits a \r\n, then puts that string
 * into the buffer.
 */
void get_line_from_socket(int socket, char *buffer)
{
    int i;
    for (i = 0; i<LINE_BUFFER_SIZE; i++) {
        char char_buffer;
        recv(socket, &char_buffer, 1, 0);
        if (char_buffer == '\r') {
            buffer[i] = '\0';
            // Read the next \n too to get rid of it
            recv(socket, &char_buffer, 1, 0);
            break;
        }
        buffer[i] = char_buffer;
    }
}

/* Builds and returns a Request struct from reading a request from the socket
 */
Request *build_request_from_socket(int socket)
{
    Request *req = malloc(sizeof(Request));
    char lineBuffer[LINE_BUFFER_SIZE];

    // Get the first request line
    RequestLine *requestLine = build_request_line_from_socket(socket);

    //Loop through each header line, add to struct as we go

    return req;
}