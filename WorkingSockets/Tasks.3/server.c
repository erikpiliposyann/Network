#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int is_palindrom(char* str){
    char* end = str;
    while(*end != '\0'){
        end++;
    }
    end--;

    while (str < end){
        if(*str != *end){
            return 0;
        }
        str++;
        end--;
    }
    return 1;
}

int main(){
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);

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

    ssize_t size = recv(client_fd, buffer, BUFFER_SIZE - 1, 0);

    if(size < 0){
        perror("recv");
        exit(EXIT_FAILURE);
    }

    buffer[size] = '\0';

    if(is_palindrom(buffer)){
        send(client_fd, "String is palindrom\n", 20, 0);
    }
    else{
        send(client_fd, "String is not palindrom\n", 24, 0);
    }

    close(client_fd);
    close(server_fd);

    return 0;
}
