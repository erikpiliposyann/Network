#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8080
#define BUFFER_SIZE 5

void* handle(void* arg){
    int client_fd = *((int*)arg);
    free(arg);

    int secret = rand() % 100 + 1;

    while(1){
        char buffer[BUFFER_SIZE] = {0};
        int size = recv(client_fd, buffer, BUFFER_SIZE - 1, 0);

        if(size < 0){
            perror("recv");
            pthread_exit(NULL);
        }

        if(size == 0){
            break;
        }

        buffer[size] = '\0';

        int guess = atoi(buffer);

        if(guess == secret){
            send(client_fd, "Correct answer.", 16, 0);
            break;
        }
        else if(guess < secret){
            send(client_fd, "Too low.", 9, 0);
        }
        else{
            send(client_fd, "Too high.", 10, 0);
        }
    }
    printf("Secret = %d\n", secret);

    close(client_fd);
    
    return NULL;
}

int main(){
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    int* newSock;
    srand(time(NULL));

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
