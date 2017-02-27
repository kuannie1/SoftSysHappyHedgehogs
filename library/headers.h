#ifndef _headers_h
#define _headers_h

#define MAX_HEADER_SIZE 101

struct _request;
struct _response;

typedef struct {
    char *field_name;
    char *field_value; //is field_content
} MessageHeader;

MessageHeader **create_headers();
MessageHeader *build_header(char *field, char *value);
void clear_headers(MessageHeader **headers, int num_headers);
void print_headers(MessageHeader **headers, int num_headers);
void add_header_to_request(struct _request *request, MessageHeader *header);
void add_header_to_response(struct _response *response, MessageHeader *header);
char *get_header(const char *field);

#endif
