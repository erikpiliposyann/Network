#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8080
#define BUFFER_SIZE 1024
#define TIME_LIMIT 10  

typedef struct Question{
    char question_arr[512];
    char opt1[128];
    char opt2[128];
    char opt3[128];
    int correct_answer;
} Question;

typedef struct {
    int fd;
    Question * question;
    int count_question;
} ClientData;

void* handle(void* arg){
    ClientData* clientdata = (ClientData*)arg;

    int client_fd = clientdata->fd;
    Question* question = clientdata->question;
    int count_question = clientdata->count_question;

    free(clientdata);

    int points = 0;

    for(int i = 0; i < count_question; ++i){
        char q_buffer[BUFFER_SIZE];

        snprintf(q_buffer, sizeof(q_buffer), "%s\n%s\n%s\n%s\nTime: %d sec", 
            question[i].question_arr, question[i].opt1,
            question[i].opt2, question[i].opt3,
            TIME_LIMIT);  

        if(send(client_fd, q_buffer, strlen(q_buffer), 0) <= 0){
            break;
        }

        struct timeval tv;
        tv.tv_sec  = TIME_LIMIT;
        tv.tv_usec = 0;
        setsockopt(client_fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));

        char a_buffer[5];
        ssize_t size = recv(client_fd, a_buffer, sizeof(a_buffer)-1, 0);

        if(size <= 0 || size >= (ssize_t)sizeof(a_buffer)){
            send(client_fd, "TIME", 4, 0);  
            continue; 
        }
        a_buffer[size] = '\0';

        int answer = atoi(a_buffer);

        if(question[i].correct_answer == answer){
            points++;
        }
    }

    char result[64];
    sprintf(result, "Your score: %d/%d", points, count_question);

    send(client_fd, result, strlen(result), 0);

    close(client_fd);

    return NULL;
}

int main(){
    FILE* stream_q = fopen("question.txt", "r");
    FILE* stream_a = fopen("answers.txt", "r");

    if(stream_q == NULL || stream_a == NULL){
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    Question question[25];
    int i = 0;
    char line[512];

    while (i < 25) {

        if (!fgets(question[i].question_arr, sizeof(question[i].question_arr), stream_q))
            break;
        question[i].question_arr[strcspn(question[i].question_arr, "\n")] = '\0';

        if (!fgets(question[i].opt1, sizeof(question[i].opt1), stream_q))
            break;
        question[i].opt1[strcspn(question[i].opt1, "\n")] = '\0';

        if (!fgets(question[i].opt2, sizeof(question[i].opt2), stream_q))
            break;
        question[i].opt2[strcspn(question[i].opt2, "\n")] = '\0';

        if (!fgets(question[i].opt3, sizeof(question[i].opt3), stream_q))
            break;
        question[i].opt3[strcspn(question[i].opt3, "\n")] = '\0';

        if (!fgets(line, sizeof(line), stream_a))
            break;

        question[i].correct_answer = atoi(line); 

        i++;
    }

    int count_question = i;

    fclose(stream_q);
    fclose(stream_a);

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if(server_fd < 0){
        perror("socket");
        exit(EXIT_FAILURE);
    }

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

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
            perror("accept");
            continue;
        }

        ClientData *data = (ClientData*) malloc(sizeof(ClientData));

        data->fd = client_fd;
        data->count_question = count_question;
        data->question = question;

        pthread_t td;
        if(pthread_create(&td, NULL, handle, data) != 0){
            perror("pthread_create");
            close(client_fd);
            free(data);
            continue;
        }
        pthread_detach(td);
    }

    return 0;
}
