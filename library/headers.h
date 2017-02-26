#ifndef _headers_h
#define _headers_h

#define MAX_HEADER_SIZE 101

struct _response;

typedef struct {
    char *field_name;
    char *field_value; //is field_content
} MessageHeader;

MessageHeader **create_headers();
void clear_headers(MessageHeader **headers);
void add_header(struct _response *response, MessageHeader *header);

#endif
