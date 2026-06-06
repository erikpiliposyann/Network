#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main(){

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0){
        perror("socket");
        return 1;
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if(connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0){
        perror("connect");
        return 1;
    }

    char buffer[BUFFER_SIZE];

    while(1){
        memset(buffer, 0, sizeof(buffer));
        ssize_t size = recv(sock, buffer, sizeof(buffer)-1, 0);

        if(size <= 0){
            printf("Server closed connection\n");
            break;
        }

        buffer[size] = '\0';

        printf("\n%s\n", buffer);

        printf("Your answer: ");
        char answer[32];
        fgets(answer, sizeof(answer), stdin);

        if(send(sock, answer, strlen(answer), 0) <= 0){
            printf("send failed\n");
            break;
        }
    }
    memset(buffer, 0, sizeof(buffer));
    ssize_t final_size = recv(sock, buffer, sizeof(buffer)-1, 0);

    if(final_size > 0){
        buffer[final_size] = '\0';
        printf("\n%s\n", buffer);
    }

    close(sock);
    return 0;
}
