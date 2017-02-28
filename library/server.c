/* Basic HTTP server library
 * Main file that the client will interact with; starts the server.
 * Created February 2017 by Sam Myers, Serena Chen, Anne Ku, and Bill Wong
 */

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "request.h"
#include "response.h"
#include "server.h"

/* Log an error and exit the program.
 *
 * message: a string to write to the console.
 */
void exit_with_error(const char *message)
{
    perror(message);
    exit(1);
}

/* Creates a socket, binds it to a port, and starts listening.
 *
 * port: a pointer to the value of the port to listen on.
 *
 * return: the file descriptor of the server socket.
 */
int setup(unsigned short port, size_t queue_size)
{
    int socket_fd = 0;
    struct sockaddr_in address = { 0 };

    socket_fd = socket(PF_INET, SOCK_STREAM, 0);
    int reset = 1;
    setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, (char*)&reset, sizeof(reset));
    if (socket_fd == -1) {
        exit_with_error("Error creating socket.");
    }
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(socket_fd, (struct sockaddr *) &address, sizeof(address)) < 0) {
        exit_with_error("Error binding to socket.");
    }
    if (listen(socket_fd, queue_size) < 0) {
        exit_with_error("Error listening on socket.");
    }
    return socket_fd;
}

/* Handles an HTTP request, then closes the connection.
 *
 * arg: a pointer to a ProcessRequestArg with information about how to handle the request
 */
void *process_request(void *arg)
{
    int socket = ((ProcessRequestArg *) arg)->client_socket;
    Application *context = ((ProcessRequestArg *) arg)->context;

    char input_buffer[BUFFER_SIZE];
    char output_buffer[BUFFER_SIZE];
    memset(&output_buffer, 0, BUFFER_SIZE);
    Response *response;

    // Retreive the request from the client socket
    Request *request = malloc(sizeof(Request));
    if (!build_request_from_socket(socket, request)) {
        response = build_response(400, "<h1>400 Bad Request</h1>\r\n");
    } else {
        char *url = request->request_line->url;
        request_type method = request->request_line->req_type;

        func_ptr endpoint_function;
        switch (get_function(context, url, method, &endpoint_function)) {
            case -1:
                // No match to this path, return a 404
                response = build_response(404, "<h1>404 Not Found</h1>\r\n");
                break;
            case -2:
                // Matching path but not matching HTTP method
                response = build_response(405, "<h1>Method Not Allowed</h1>\r\n");
                break;
            case 0:
                // Process the request
                response = endpoint_function(request);
                break;
        }
    }
    response_struct_to_str(response, output_buffer);

    // Send back a response
    send(socket, output_buffer, BUFFER_SIZE, 0);

    clear_request(request);
    clear_response(response);
    close(socket);
}

/* Creates an Application, which keeps track of a port, and queue size, and
 * allocates an array to map endpoints to functions
 *
 * port: port number
 * queue_size: the number of connections to accept at a time
 *
 * returns: a pointer to the created Application struct
 */
Application *create_application(unsigned short port, size_t queue_size)
{
    Endpoint **endpoints = malloc(MAX_ENDPTS * sizeof(Endpoint *));
    func_ptr *functions = malloc(MAX_ENDPTS * sizeof(func_ptr *));

    Application *app = malloc(sizeof(Application));
    *app = (Application) { port, queue_size, endpoints, functions, 0 };

    return app;
}

/* Registers an endpoint function with the application.
 *
 * app: an initialized Application struct.
 * path: the URL pattern to match to this endpoint.
 * method: the HTTP method to match to this endpoint.
 * function: a pointer to the function to register.
 */
void register_endpoint(Application *app, const char *path, request_type method,
                       func_ptr function)
{
    Endpoint *endpoint = malloc(sizeof(Endpoint));
    *endpoint = (Endpoint) { path, method };

    app->endpoints[app->num_endpoints] = endpoint;
    app->functions[app->num_endpoints] = function;
    app->num_endpoints++;
}

/* Finds the registered endpoint function for a given path and method.
 *
 * app: a pointer to the Application struct.
 * path: the URI to match.
 * method: the HTTP method to match.
 * function: a memory address to assign the matched function pointer to.
 *
 * returns: an int indicating the success of this operation.
 *          nonzero return value indicates that the endpoint does not exist.
 */
int get_function(Application *app, const char *path, request_type method,
                 func_ptr *function)
{
    size_t i = 0;
    bool path_found = false;
    bool method_found = false;

    while (i < app->num_endpoints && !(path_found && method_found)) {
        if (strcasecmp(path, app->endpoints[i]->path) == 0) {
            path_found = true;

            if (app->endpoints[i]->method == method) {
                method_found = true;
                continue;
            }
        }
        i++;
    }

    if (i >= app->num_endpoints) {
        if (path_found) {
            return -2;
        } else {
            return -1;
        }
    } else {
        *function = app->functions[i];
        return 0;
    }
}

/*
 * Main library function that the user will call. Starts the server by opening the
 * socket and runs continuously, taking in clients that connect and using the
 * passed in server_logic function to process the clients' requests.
 *
 * app: pointer to Application struct that maps functions to endpoints
 */
void start_server(Application *app)
{
    int server_socket = -1;
    int client_socket = -1;

    struct sockaddr_in client_address;
    int address_len = sizeof(client_address);

    pthread_t new_thread;

    server_socket = setup(app->port, app->queue_size);
    printf("HTTP Server running on port %d\n", app->port);

    // Main program loop; accepts and handles connections from the queue
    while (true) {
        client_socket = accept(server_socket, (struct sockaddr *) &client_address, &address_len);
        if (client_socket == -1) {
            exit_with_error("Error accepting connection.");
        }

        ProcessRequestArg arg = { client_socket, app };

        if (pthread_create(&new_thread, NULL, process_request, &arg) != 0) {
            perror("Error creating thread.");
        }
    }
}
