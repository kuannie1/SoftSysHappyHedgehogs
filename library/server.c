/* Basic HTTP server library
 * Created February 2017 by Sam Myers, Serena Chen, Anne Ku, and Bill Wong
 */

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "structs.h"

#define QUEUE_SIZE 10
#define PORT 8080
#define BUFFER_SIZE 1024

typedef struct {
    int client_socket;
    Response * (*server_logic) (char *input_buffer);
} ProcessRequestArg;

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
int setup(u_short *port)
{
    int socket_fd = 0;
    struct sockaddr_in address = { 0 };

    socket_fd = socket(PF_INET, SOCK_STREAM, 0);
    if (socket_fd == -1) {
        exit_with_error("Error creating socket.");
    }
    address.sin_family = AF_INET;
    address.sin_port = htons(*port);
    address.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(socket_fd, (struct sockaddr *) &address, sizeof(address)) < 0) {
        exit_with_error("Error binding to socket.");
    }
    if (listen(socket_fd, QUEUE_SIZE) < 0) {
        exit_with_error("Error listening on socket.");
    }
    return socket_fd;
}

void response_struct_to_str(Response *res, char *output_buffer)
{
    size_t num_headers = sizeof(res->headers);
    char all_headers_str[BUFFER_SIZE];
    for(int i = 0; i < num_headers; i++){
        MessageHeader header = res->headers[i];
        char *header_str[BUFFER_SIZE];
        sprintf(header_str, "%s: %s", header.field_name, header.field_value);
        strcat(all_headers_str, header_str);
    }
    // StatusLine status_line = res->status_line;
    sprintf(output_buffer, "%s %i %s\r\n%s\r\n%s\r\n",
                                            res->status_line->http_ver,
                                            res->status_line->status->code,
                                            res->status_line->status->reason_phrase,
                                            all_headers_str,
                                            res->body);
}

/* Handles an HTTP request, then closes the connection.
 *
 * arg: a pointer to a ProcessRequestArg with information about how to handle the request
 */
void *process_request(void *arg)
{
    ProcessRequestArg *request_arg = (ProcessRequestArg *) arg;

    char input_buffer[BUFFER_SIZE];
    char output_buffer[BUFFER_SIZE];

    // Retreive the message from the client socket and load into the input buffer
    recv(request_arg->client_socket, input_buffer, BUFFER_SIZE, 0);

    // Process the request
    Response *res = request_arg->server_logic(input_buffer);
    response_struct_to_str(res, output_buffer);

    // Send back a response
    send(request_arg->client_socket, output_buffer, BUFFER_SIZE, 0);
    send(request_arg->client_socket, "\n", 1, 0);

    close(request_arg->client_socket);
}

/*
 * Main library function that the user will call. Starts the server by opening the
 * socket and runs continuously, taking in clients that connect and using the
 * passed in server_logic function to process the clients' requests.
 *
 * server_logic: function supplied by user for server logic. Server logic
 *          functions should take in an input string and pack an output buffer.
 * Errors may occur is things happen.
 */
void start_server(Response * (*server_logic)(char *))
{
    int server_socket = -1;
    int client_socket = -1;
    u_short port = PORT;

    struct sockaddr_in client_address;
    int address_len = sizeof(client_address);

    pthread_t new_thread;

    server_socket = setup(&port);
    printf("HTTP Server running on port %d\n", port);

    // Main program loop; accepts and handles connections from the queue
    while (true) {
        client_socket = accept(server_socket, (struct sockaddr *) &client_address, &address_len);
        if (client_socket == -1) {
            exit_with_error("Error accepting connection.");
        }

        ProcessRequestArg arg = { client_socket, server_logic };

        if (pthread_create(&new_thread, NULL, process_request, &arg) != 0) {
            perror("Error creating thread.");
        }
    }
}

/*
 * Shifts all ascii characters of the input string by one, and writes to the output buffer
 *
 * input_buffer: input string
 *
 * return: pointer to Response struct with the cipher as the body
 */
Response *caesar_cipher(char *input_buffer)
{
    char caesar[sizeof(input_buffer)];
    for (int i=0; i<sizeof(input_buffer); i++) {
        caesar[i] = input_buffer[i] + 1;
    }
    Response *res = build_response(200, caesar);
    return res;
}

/* Writes a super basic 200 response with an HTML page to the buffer.
 *
 * input_buffer: input string
 *
 * return: pointer to Response struct with "Sup" as the body
 */
Response *write_html_page(char *input_buffer)
{
    Response *res = build_response(200, "Sup");
    return res;
}
int main(void)
{
    start_server(write_html_page);
    return 0;
}
