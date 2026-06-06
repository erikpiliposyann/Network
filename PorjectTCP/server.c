#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8080
#define BUFFER_SIZE 1024
#define NAME_SIZE 50

typedef struct Product{
    char name[NAME_SIZE];
} Product;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void init_db(){
    FILE* db = fopen("products.db", "ab+");

    if(db == NULL){
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    fclose(db);
}

void post(int client_fd, char* buffer);
void get_product(int client_fd, char* buffer);
void get_all(int client_fd);


void* handle(void* arg){
    int client_fd = *((int*)arg);
    free(arg);

    char buffer[BUFFER_SIZE] = {0};

    ssize_t size = recv(client_fd, buffer, BUFFER_SIZE-1, 0);

    if(size <= 0){
        perror("recv");
        close(client_fd);
        return NULL;
    }
    buffer[size] = '\0';

    if(strncmp(buffer, "POST ", 5) == 0){
        post(client_fd,buffer);
    }
    else if(strncmp(buffer, "GET ", 4) == 0){
        get_product(client_fd, buffer);
    }
    else if(strncmp(buffer, "GET", 3) == 0){
        get_all(client_fd);
    }
    

    close(client_fd);
    
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

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

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

    init_db();

    while(1){
        int client_fd = accept(server_fd, (struct sockaddr*) &client_addr, &client_len);
        
        if(client_fd < 0){
            perror("accpet");
            continue;
        }

        newSock = (int*) malloc(sizeof(int));
        *newSock = client_fd;

        pthread_t td;
        pthread_create(&td, NULL, handle, newSock); 
        pthread_detach(td);
    }

    return 0;
}

void post(int client_fd, char* buffer) {
    pthread_mutex_lock(&mutex);

    FILE* db = fopen("products.db", "ab");
    
    if(db == NULL){
        pthread_mutex_unlock(&mutex);
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    Product p;
    memset(&p, 0, sizeof(Product));

    strncpy(p.name, buffer + 5, NAME_SIZE -1);
    fwrite(&p, sizeof(Product), 1, db);
    
    fclose(db);

    pthread_mutex_unlock(&mutex);

    if(send(client_fd, "Product successfully added\n", 26, 0) < 0){
        perror("send");
        return;
    }
}

void get_product(int client_fd, char *buffer){
    pthread_mutex_lock(&mutex);
    FILE* db = fopen("products.db", "rb");

    if(db == NULL){
        pthread_mutex_unlock(&mutex);
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    Product p;
    while(fread(&p, sizeof(Product), 1, db) == 1) {
        if(strncmp(p.name, buffer+4, NAME_SIZE) == 0){
            send(client_fd, p.name, strlen(p.name), 0);
            send(client_fd, "\n", 1, 0);
            fclose(db);
            pthread_mutex_unlock(&mutex);
           return;
        }
    }
    if(send(client_fd, "Not found product", 18, 0) < 0){
        fclose(db);
        pthread_mutex_unlock(&mutex);
        perror("send");
        return;
    }
    
    fclose(db);
    pthread_mutex_unlock(&mutex);
}

void get_all(int client_fd) {
    pthread_mutex_lock(&mutex);
    FILE* db = fopen("products.db", "rb");

    if(db == NULL){
        pthread_mutex_unlock(&mutex);
        perror("fopen");
        exit(EXIT_FAILURE);
    }
    Product p;
    int found = 0;
    while(fread(&p, sizeof(Product), 1, db) == 1) {
        found = 1;
        send(client_fd, p.name, strlen(p.name), 0);
        send(client_fd, "\n", 1, 0);
    }
    if(found == 0){
        send(client_fd, "Empty\n", 6, 0);
    }
    fclose(db);
    pthread_mutex_unlock(&mutex);
}
