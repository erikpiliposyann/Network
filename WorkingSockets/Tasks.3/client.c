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

    if(client_fd < 0){
        perror("socket");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in client_addr;

    client_addr.sin_family = AF_INET;
    client_addr.sin_port = htons(PORT);

    inet_pton(AF_INET, "127.0.0.1", &client_addr.sin_addr);

    if(connect(client_fd, (struct sockaddr*) &client_addr, sizeof(client_addr)) < 0){
        perror("connect");
        exit(EXIT_FAILURE);
    }
    char msg[] = "helleho";

    if(send(client_fd, msg, sizeof(msg), 0) < 0){
        perror("send");
        exit(EXIT_FAILURE);
    }

    char buffer[BUFFER_SIZE] = {0};

    ssize_t size = recv(client_fd, buffer, BUFFER_SIZE - 1, 0);

    if(size < 0){
        perror("recv");
        exit(EXIT_FAILURE);
    }
    buffer[size] = '\0';

    printf("%s", buffer);
    
    return 0;
}
