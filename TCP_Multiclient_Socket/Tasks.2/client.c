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
    socklen_t client_size = sizeof(client_addr);

    if(connect(client_fd, (struct sockaddr*) &client_addr, client_size) < 0){
        perror("connect");
        exit(EXIT_FAILURE);
    }
    
    printf("Let's go.");
    while(1){
        printf("Input number: ");

        int n = 0;
        scanf("%i", &n);
    
        char buffer[BUFFER_SIZE];
        sprintf(buffer, "%d", n);
        if(send(client_fd, buffer, strlen(buffer), 0) < 0){
            perror("send");
            exit(EXIT_FAILURE);
        }

        int size = recv(client_fd, buffer, BUFFER_SIZE-1, 0);
        
        if(size < 0){
            perror("recv");
            exit(EXIT_FAILURE);
        }

        buffer[size] = '\0';
        printf("Server: %s\n", buffer);

        if(strcmp(buffer,"Correct answer.") == 0){
            break;
        }
    }
    close(client_fd);

    return 0;
}
