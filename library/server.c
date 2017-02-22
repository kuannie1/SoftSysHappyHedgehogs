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
} process_request_arg;

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
    process_request_arg *request_args = (process_request_arg *) arg;

    char input_buffer[BUFFER_SIZE];
    char output_buffer[BUFFER_SIZE];

    // Retreive the message from the client socket and load into the input buffer
    int message_length = recv(request_args->client_socket, input_buffer, BUFFER_SIZE, 0);

    // Process the request and send back a response
    (request_args->server_logic)(input_buffer, output_buffer);
    send(request_args->client_socket, output_buffer, BUFFER_SIZE, 0);
    send(request_args->client_socket, "\n", 1, 0);

    close(request_args->client_socket);
}

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

        process_request_arg arg = { client_socket, server_logic };

        if (pthread_create(&new_thread, NULL, process_request, &arg) != 0) {
            perror("Error creating thread.");
        }
    }
}

void alphabet_shift_char(char *input_buffer, char *output_buffer)
{
    for (int i=0; i<sizeof(input_buffer); i++) {
        output_buffer[i] = input_buffer[i] + 1;
    }
}

int main(void)
{
    start_server(alphabet_shift_char);
    return 0;
}
