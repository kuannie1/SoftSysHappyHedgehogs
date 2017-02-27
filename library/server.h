#ifndef _server_h
#define _server_h

#include <stdlib.h>
#include "request.h"
#include "response.h"

#define MAX_ENDPTS 100
#define MAX_ENDPT_LEN 255

typedef Response * (*func_ptr)(Request *);

typedef struct {
    char *path;
    request_type method;
} Endpoint;

typedef struct {
    unsigned short port;
    size_t queue_size;
    Endpoint **endpoints;
    func_ptr *functions;
    int num_endpoints;
} Application;

typedef struct {
    int client_socket;
    Application *context;
} ProcessRequestArg;

Application *create_application(unsigned short port, size_t queue_size);
void register_endpoint(Application *app, const char *path, request_type method,
                       func_ptr function);
void start_server(Application *app);

#endif
