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

#define QUEUE_SIZE 10
#define PORT 8080
#define BUFFER_SIZE 1024

typedef struct {
    int client_socket;
    void (*server_logic) (char *input_buffer, char *output_buffer);
} ProcessRequestArg;


void exit_with_error(const char *message)
{
    perror(message);
    exit(1);
}

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

// Handles an HTTP request
void *process_request(void *arg)
{
    ProcessRequestArg *request_arg = (ProcessRequestArg *) arg;

    char input_buffer[BUFFER_SIZE];
    char output_buffer[BUFFER_SIZE];

    // Retreive the message from the client socket and load into the input buffer
    recv(request_arg->client_socket, input_buffer, BUFFER_SIZE, 0);

    // Process the request and send back a response
    (request_arg->server_logic)(input_buffer, output_buffer);
    send(request_arg->client_socket, output_buffer, BUFFER_SIZE, 0);
    send(request_arg->client_socket, "\n", 1, 0);

    close(request_arg->client_socket);
}

/*
Main library function that the user will call. Starts the server by opening the
socket and runs continuously, taking in clients that connect and using the
passed in server_logic function to process the clients' requests.

server_logic: function supplied by user for server logic. Server logic
            functions should take in an input string and pack an output buffer.
Errors may occur is things happen.
*/
void start_server(void (*server_logic)(char *, char *))
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
Shifts all ascii characters of the input string by one, and writes to the output buffer

input_buffer: input string
output_buffer: output string
*/
void caesar_cipher(char *input_buffer, char *output_buffer)
{
    for (int i=0; i<sizeof(input_buffer); i++) {
        output_buffer[i] = input_buffer[i] + 1;
    }
}

int main(void)
{
    start_server(caesar_cipher);
    return 0;
}
