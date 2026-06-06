#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8080
#define BUFFER_SIZE 1024

void reverse_string(char* str){
    char *end = str;

    while (*end != '\0') {
        end++;
    }
    end--;  

    while(str < end){
        char temp = *str;
        *str = *end;
        *end = temp;
        str++;
        end--;
    }
}

int main(){
    int server_fd;

    struct sockaddr_in server_addr;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    
    if(server_fd < 0){
        perror("socket");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if(bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0){
        perror("bind");
        exit(EXIT_FAILURE);
    }

    if(listen(server_fd, SOMAXCONN) < 0){
        perror("listen");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);

    int client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);

    if(client_fd < 0){
        perror("accept");
        exit(EXIT_FAILURE);
    }

    char buf[BUFFER_SIZE];
    ssize_t n = recv(client_fd, buf, BUFFER_SIZE - 1, 0);

    if(n < 0){
        perror("recv");
        exit(EXIT_FAILURE);
    }

    buf[n] = '\0';

    reverse_string(buf);

    ssize_t sent = send(client_fd, buf, strlen(buf), 0);
    
    if(sent < 0){
        perror("send");
        exit(EXIT_FAILURE);
    }

    close(client_fd);
    close(server_fd);

    return 0;
}
