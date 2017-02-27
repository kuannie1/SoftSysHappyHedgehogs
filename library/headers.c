/* Basic HTTP server library
 * Collection of methods to create and manipulate MessageHeader structs.
 * Created February 2017 by Sam Myers, Serena Chen, Anne Ku, and Bill Wong
 */

#include <stdlib.h>
#include "headers.h"
#include "request.h"
#include "response.h"

/* Allocates the array of headers
 *
 * returns: the pointer to the array of MessageHeaders
 */
MessageHeader **create_headers()
{
    MessageHeader **headers = malloc(MAX_HEADER_SIZE * sizeof(MessageHeader *));
    return headers;
}

/* Allocates a MessageHeader and fills it with the fields specified
 *
 * field: header key
 * value: header value
 *
 * returns: pointer to MessageHeader
 */
MessageHeader *build_header(char *field, char *value)
{
    MessageHeader *header = malloc(sizeof(MessageHeader));
    *header = (MessageHeader) { field, value };
    return header;
}

/* Frees all the memory associated with pointers
 *
 * headers: pointer to the array of MessageHeaders
 * num_headers: the number of headers contained in the array
 */
void clear_headers(MessageHeader **headers, int num_headers)
{
    size_t i;
    for (i = 0; i < num_headers; i++) {
        free(headers[i]);
    }
    free(headers);
}

/* Add a MessageHeader to the array of headers in a Request struct
 *
 * request: Request to add the header to
 * header: the header to add
 */
void add_header_to_request(Request *request, MessageHeader *header)
{
    request->headers[request->num_headers] = header;
    request->num_headers++;
}

/* Add a MessageHeader to the array of headers in a Response struct
 *
 * respopnse: Response to add the header to
 * header: the header to add
 */
void add_header_to_response(Response *response, MessageHeader *header)
{
    response->headers[response->num_headers] = header;
    response->num_headers++;
}
