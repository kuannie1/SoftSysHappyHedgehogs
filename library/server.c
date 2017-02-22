#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#define QUEUE_SIZE 10
#define PORT 8080

typedef struct{
    int client_socket;
    void (*server_logic) (char *input, char *response_buffer);
}process_request_arg;

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

void process_request(process_request_arg arg)
{
    // Handle an HTTP request

    size_t str_size = 100;
    char *input[str_size];
    fgets(input, str_size, arg.client_socket); // probably a socket specific function for this
    char *output_buffer[str_size];
    //write to buffer
    (arg.server_logic)(input, output_buffer);
    //send buffer to socket using send()
    send(arg.client_socket, output_buffer, str_size, 0);
}

void start_server(void (*server_logic) (char *input, char *response_buffer)){
    int server_socket = -1;
    int client_socket = -1;
    u_short port = PORT;

    struct sockaddr_in client_address;
    int address_len = sizeof(client_address);

    pthread_t new_thread;

    server_socket = setup(&port);
    printf("HTTP Server running on port %d\n", port);



    while (true) {
        client_socket = accept(server_socket, (struct sockaddr *) &client_address, &address_len);
        if (client_socket == -1) {
            exit_with_error("Error accepting connection.");
        }

        if (pthread_create(&new_thread, NULL, &process_request, &((process_request_arg) {client_socket, server_logic})) != 0) {
            perror("Error creating thread.");
        }
    }

}

void random_char(char *input, char *output_buffer){
    for(int i = 0; i<sizeof(input); i++){
        output_buffer[i] = input[i] + 1;
    }
}

int main(void)
{
    start_server(random_char);
    return 0;
}
