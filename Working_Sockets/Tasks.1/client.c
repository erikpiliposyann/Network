#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main(){
    int client_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (client_fd < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_addr;

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);

    int con = connect(client_fd, (struct sockaddr*) &server_addr, sizeof(server_addr));

    if(con < 0){
        perror("connect");
        exit(EXIT_FAILURE);
    }

    char buffer[BUFFER_SIZE];

    char msg[] = "hello";

    if(send(client_fd, msg, strlen(msg), 0) < 0){
        perror("send");
        exit(EXIT_FAILURE);
    }

    size_t n = recv(client_fd, buffer, BUFFER_SIZE - 1, 0);

    if(n < 0){
        perror("recv");
        exit(EXIT_FAILURE);
    }
    buffer[n] = '\0';

    printf("Server says: %s\n", buffer);

    close(client_fd);

    return 0;
}
