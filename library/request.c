/* Basic HTTP server library
 * Collection of methods to read in and manipulate Request structs.
 * Created February 2017 by Sam Myers, Serena Chen, Anne Ku, and Bill Wong
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "request.h"

/* Converts a string to a request_type enum
 *
 * str: String that contains the request type, such as HEAD, POST, GET, PUT,
 *                    DELETE, OPTIONS, CONNECT
 *
 * returns: request_type enum that represents the string
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

/* Reads all the remaining data on the socket to clear it. Does not store it
 * anywhere.
 */
void flush_socket(int socket)
{
    fd_set set;
    struct timeval tv;

    // Set timeout to .01 seconds
    tv.tv_sec = 0;
    tv.tv_usec = 10000;

    // Add socket to to file descriptor set
    FD_SET(socket, &set);

    // Read infinitely until the socket has no more info or recv gives an error
    for (;;) {
        char buffer[BUFFER_SIZE];
        if (select(socket+1, &set, NULL, NULL, &tv) < 1) {
            // Socket has no more information
            return;
        }
        if (recv(socket, buffer, BUFFER_SIZE, 0) < 1) {
            // recv gave an error
            return;
        }
    }
}

/* Reads the socket char-by-char to the buffer until it hits the stopper
 * character or an end of line.
 *
 * socket: socket file descriptor to read
 * stopper: stopper character
 * buffer: String to pack
 *
 * returns: 1 if read sucessfully, 0 if request was too large or another
 *          recv error occurred.
 */
int read_socket_until_stopper(int socket, char stopper, char *buffer, int length)
{
    int i;
    fd_set set;
    struct timeval tv;

    // Set timeout to .01 seconds
    tv.tv_sec = 0;
    tv.tv_usec = 10000;

    // Add socket to to file descriptor set
    FD_SET(socket, &set);

    for (i = 0; i<length; i++) {
        char *char_buffer = malloc(sizeof(char));

        if (select(socket+1, &set, NULL, NULL, &tv) < 1) {
            // Socket has no more information
            buffer[i] = '\0';
            return 1;
        }
        if (recv(socket, char_buffer, 1, 0) < 1) {
            // recv gave an error
            buffer[i] = '\0';
            return 0;
        }
        if (*char_buffer == stopper || *char_buffer == '\r') {
            // We've reached the stopper char or the end of the line
            buffer[i] = '\0';
            if (*char_buffer == '\r') {
                // Read the next \n too to get rid of it
                recv(socket, char_buffer, 1, 0);
            }
            return 1;
        }
        buffer[i] = *char_buffer;
    }
    // We reached the end of the buffer length before we hit any stop conditions
    buffer[length] = '\0';
    flush_socket(socket);
    return 0;
}

/* Assumes that the socket is currently at the request line. Builds a RequestLine
 * struct by reading from the socket word-by-word.
 *
 * socket: socket file descriptor to read
 * request_line: pointer to RequestLine struct to modify
 *
 * returns: 1 if successful, 0 if there's an error reading the socket.
 */
int build_request_line_from_socket(int socket, RequestLine *request_line)
{
    char type_buffer[MAX_REQ_TYPE_SIZE];
    char *url_buffer = malloc(LINE_BUFFER_SIZE * sizeof(char));
    char *version_buffer = malloc(MAX_VERSION_SIZE * sizeof(char));

    if (!read_socket_until_stopper(socket, ' ', type_buffer, MAX_REQ_TYPE_SIZE))
        return 0;
    request_line->req_type = request_type_string_to_enum(type_buffer);

    if (!read_socket_until_stopper(socket, ' ', url_buffer, LINE_BUFFER_SIZE))
        return 0;
    request_line->url = url_buffer;

    if (!read_socket_until_stopper(socket, '\r', version_buffer, MAX_VERSION_SIZE))
        return 0;
    request_line->http_ver = version_buffer;

    return 1;
}

/* Builds and returns a MessageHeader struct from the source string.
 *
 * str: source string in the format "name: value"
 *
 * returns: the MessageHeader that the string represents
 */
MessageHeader *build_header_from_string(char *str)
{
    char *copy = malloc(sizeof(char) * strlen(str));
    char *name = malloc(sizeof(char) * strlen(str));
    char *val = malloc(sizeof(char) * strlen(str));

    strcpy(copy, str);
    name = strtok(copy, ":");
    val = strtok(NULL, "");

    // Get rid of leading space
    val = val + 1;
    return build_header(name, val);
}

/* Builds a Request struct from reading a request from the socket.
 *
 * socket: socket file descriptor to read
 * req: Request struct to modify
 *
 * returns: 1 if successful, 0 if request could not be parsed
 */
int build_request_from_socket(int socket, Request *req)
{
    // Request *req = malloc(sizeof(Request));
    char *body = malloc(sizeof(char) * BUFFER_SIZE);
    MessageHeader **headers = create_headers();
    RequestLine *request_line = malloc(sizeof(RequestLine));

    if (!build_request_line_from_socket(socket, request_line))
        return 0;

    *req = (Request) { request_line, headers, 0, body };

    //Loop through each header line, add to struct as we go
    for (;;) {
        char line_buffer[LINE_BUFFER_SIZE];
        if (!read_socket_until_stopper(socket, '\r', line_buffer, LINE_BUFFER_SIZE))
            return 0;

        // If buffer is empty, we're done with headers
        if (!line_buffer[0]) break;
        add_header_to_request(req, build_header_from_string(line_buffer));
    }

    // The rest of the socket is the body
    if (!read_socket_until_stopper(socket, '\r', body, BUFFER_SIZE))
        return 0;

    return 1;
}

/* de-allocates all the memory allocated in the struct
 *
 * request: Request struct to destroy
 */
void clear_request(Request *request)
{
    free(request->request_line->url);
    free(request->request_line->http_ver);
    free(request->request_line);
    clear_headers(request->headers, request->num_headers);
    free(request->body);
    free(request);
}
