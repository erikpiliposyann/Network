#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8080
#define BUFFER_SIZE 1024

void* handle(void* arg){
    int fd = *((int*)arg);
    free(arg);
    close(fd);
    
    return NULL;
}

int main(){
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    int* newSock;

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

    while(1){
        int client_fd = accept(server_fd, (struct sockaddr*) &client_addr, &client_len);
        
        if(client_fd < 0){
            perror("accpet");
            exit(EXIT_FAILURE);
        }

        newSock = (int*) malloc(sizeof(int));
        *newSock = client_fd;

        pthread_t td;
        pthread_create(&td, NULL, handle, newSock); 
        pthread_detach(td);
    }

    return 0;
}
