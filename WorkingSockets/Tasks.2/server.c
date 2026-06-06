#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main(){
    int server_fd = socket(AF_INET, SOCK_STREAM,0);

    if(server_fd < 0){
        perror("socket");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_addr;

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if(bind(server_fd, (struct sockaddr*) &server_addr, sizeof(server_addr)) < 0){
        perror("bind");
        exit(EXIT_FAILURE);
    }

    if(listen(server_fd, SOMAXCONN) < 0){
        perror("listen");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);

    int client_fd = accept(server_fd, (struct sockaddr*) &client_addr, &client_len);

    if(client_fd < 0){
        perror("accept");
        exit(EXIT_FAILURE);
    }

    char buffer[BUFFER_SIZE] = {0};

    ssize_t n = recv(client_fd, buffer, BUFFER_SIZE - 1, 0);

    if(n < 0){
        perror("recv");
        exit(EXIT_FAILURE);
    }
    buffer[n] = '\0';

    FILE* fp = fopen(buffer,"r");

    if(fp == NULL){
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    printf("Requested file: %s\n", buffer);

    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    char* data = (char*) malloc((size+1) * sizeof(char));
    long data_size = fread(data, 1, size, fp);

    if (data_size != size) {
        perror("fread");
        exit(EXIT_FAILURE);
    }   

    data[data_size] = '\0';

    ssize_t sent = send(client_fd, data, data_size, 0);

    if(sent < 0){
        perror("send");
        exit(EXIT_FAILURE);
    }

    fclose(fp);
    free(data);

    close(client_fd);
    close(server_fd);

    return 0;
}
