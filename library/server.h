#ifndef _server_h
#define _server_h

#include "request.h"
#include "response.h"

#define MAX_ENDPTS 100
#define MAX_ENDPT_LENGTH 255

typedef struct {
    int client_socket;
    Response * (*server_logic)(char *input_buffer);
} ProcessRequestArg;

typedef Response * (*func_ptr)(Request *request);

typedef struct {
    u_short port;
    size_t queue_size;
    char **endpoints;
    func_ptr *methods;
    int num_endpoints;
} Application;

Application *create_application(u_short port, size_t queue_size);
void register_endpoint(Application *app, const char *path, func_ptr *method);
void start_server(Application *app);

#endif
