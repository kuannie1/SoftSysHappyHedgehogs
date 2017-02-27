/* Basic HTTP server library
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
int setup(u_short port, size_t queue_size)
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
    Application *context = ((ProcessRequestArg *) arg)->application;

    char input_buffer[BUFFER_SIZE];
    char output_buffer[BUFFER_SIZE];
    memset(&output_buffer, 0, BUFFER_SIZE);

    // Retreive the request from the client socket
    Request *request = build_request_from_socket(socket);

    func_ptr *method;
    Response *response;
    if (get_method(context, request->request_line->url, &method) != 0) {
        // No match to this path, return a 404
        response = build_response(404, "<h1>404 Not Found</h1>");
    } else {
        // Process the request
        response = endpoint_method(request);
    }
    response_struct_to_str(response, output_buffer);

    // Send back a response
    send(socket, output_buffer, BUFFER_SIZE, 0);

    clear_response(res);
    close(request_arg->client_socket);
}

Application *create_application(u_short port, size_t queue_size)
{
    char **endpoints = malloc(MAX_ENDPTS * MAX_ENDPT_LEN * sizeof(char));
    func_ptr *methods = malloc(MAX_ENDPTS * sizeof(func_ptr *));

    Application *app = malloc(sizeof(Application));
    *app = (Application) { port, queue_size, endpoints, methods, 0 };

    return app;
}

void register_endpoint(Application *app, const char *path, func_ptr *method)
{
    app->endpoints[app->num_endpoints] = path;
    app->methods[app->num_endpoints] = method;
    app->num_endpoints++;
}

int get_method(Application *app, const char *path, func_ptr **method)
{
    size_t i = 0;
    while (i < app->num_endpoints && strcasecmp(path, app->endpoints[i]))
        i++;

    if (i >= app->num_endpoints) {
        return -1;
    } else {
        *method = app->methods[i];
        return 0;
    }
}

/*
 * Main library function that the user will call. Starts the server by opening the
 * socket and runs continuously, taking in clients that connect and using the
 * passed in server_logic function to process the clients' requests.
 *
 * Errors may occur is things happen.
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
