#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

#define SIZE_BUF 100

int reg_login(int server_fd, char cmd[]){
    int ret_val, isFound = 0;
    char id[SIZE_BUF], pass[SIZE_BUF], mssg[SIZE_BUF];
    printf("Username\n");
    scanf("%s", id);
    ret_val = send(server_fd, id, SIZE_BUF, 0);
    printf("pass\n");
    scanf("%s", pass);
    ret_val = send(server_fd, pass, SIZE_BUF, 0);
    ret_val = recv(server_fd, mssg, SIZE_BUF, 0);
    puts(mssg);
    if(!strcmp(mssg, "regloginsuccess\n")) return 1;
    else if(!strcmp(mssg, "userfound\n")) {
        printf("Username or ID already exist !\n\n");
        return 0;
    }
    else if(!strcmp(mssg, "wrongpass\n")) {
        printf("Id or passsword doesn't match !\n\n");
        return 0;
    }
}
void sendFile(int sockserver_fd, char filePath[]){
    int n;
    char data[SIZE_BUF] = {0};
    FILE *fp = fopen(filePath, "r");
    while(fgets(data, SIZE_BUF, fp) != NULL) {
        if (send(sockserver_fd, data, sizeof(data), 0) == -1) {
            perror("[-]Error in sending file.");
            exit(1);
        }
        bzero(data, SIZE_BUF);
    }
    fclose(fp);
    int ret_val = send(sockserver_fd, "done", SIZE_BUF, 0);
}
int main () {
    struct sockaddr_in saddr;
    int server_fd, ret_val;
    struct hostent *local_host;
    char mssg[SIZE_BUF],  cmd[SIZE_BUF];
    server_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (server_fd == -1) {
        fprintf(stderr, "socket failed [%s]\n", hstrerror(errno));
        return -1;
    }
    printf("Created a socket with server_fd: %d\n", server_fd);
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(7000);
    local_host = gethostbyname("127.0.0.1");
    saddr.sin_addr = *((struct in_addr *)local_host->h_addr);
    ret_val = connect(server_fd, (struct sockaddr *)&saddr, sizeof(struct sockaddr_in));
    if (ret_val == -1) {
        fprintf(stderr, "connect failed [%s]\n", hstrerror(errno));
        close(server_fd);
        return -1;
    }
    ret_val = recv(server_fd, mssg, SIZE_BUF, 0);
    while(strcmp(mssg, "wait") == 0) {
        printf("\e[31mServer is full!\e[0m\n");
        ret_val = recv(server_fd, mssg, SIZE_BUF, 0);
    }
    int commandTrue = 0;
    while(1) {
        while(!commandTrue) {
            printf("Insert Command (Register/Login)\n");
            scanf("%s", cmd);
            int b=0;
            for(b=0;b<strlen(cmd);b++) cmd[b] = tolower(cmd[b]);
            ret_val = send(server_fd, cmd, SIZE_BUF, 0);
            if(!strcmp(cmd, "register") || !strcmp(cmd, "login")) {
                if(reg_login(server_fd, cmd))
                    commandTrue = 1;
            }
	    else {
                ret_val = recv(server_fd, mssg, SIZE_BUF, 0);
                if(!strcmp(mssg, "notlogin\n")) printf("Invalid Input\n\n");
                else commandTrue = 1;
            }
        }
        sleep(2);
        if(commandTrue) break;
    }
    printf("Exit");
    close(server_fd);
    return 0;
}
