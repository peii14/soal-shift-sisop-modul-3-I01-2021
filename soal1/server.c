#include <stdio.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/time.h>
#include <dirent.h>
#include <stdlib.h>

#define SIZE 100
#define DATA_BUFF 50
#define MAX_CONNECT 10

int make_tcp_server_socket() {
    struct sockaddr_in saddr;
    int fd, ret_val;
    fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (fd == -1) {
        fprintf(stderr, "socket failed [%s]\n", strerror(errno));
        return -1;
    }
    printf("Created a socket with fd: %d\n", fd);
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(7000);
    saddr.sin_addr.s_addr = INADDR_ANY;
    ret_val = bind(fd, (struct sockaddr *)&saddr, sizeof(struct sockaddr_in));
    if (ret_val != 0) {
        fprintf(stderr, "bind failed [%s]\n", strerror(errno));
        close(fd);
        return -1;
    }
    ret_val = listen(fd, 5);
    if (ret_val != 0) {
        fprintf(stderr, "listen failed [%s]\n", strerror(errno));
        close(fd);
        return -1;
    }
    return fd;
}
//A
int idPass_Check(char id[], char pass[], char cmd[]){
	char line[512];
	const char delim[2] = ":";
	char *tempId, *tempPass;
	FILE *fp = fopen("akun.txt", "r");
	while(fgets(line, 512, fp)){
		char *newline = strchr( line, '\n' );
		if ( newline )
			*newline = 0;
		tempId = strtok(line, delim);
		tempPass = strtok(NULL, delim);
		if(!strcmp(cmd, "register")){
			if(!strcmp(tempId, id)) return 1;
		}
		else{
			if(!strcmp(tempId, id) && !strcmp(tempPass, pass)) return 1;
		}
	}
	fclose(fp);
	return 0;
}
void register_login(int all_connections_i, char cmd[], char id[], char pass[],int *userLoggedIn, int all_connections_serving ) {
    int ret_val;
    int status_val;
    if(!strcmp(cmd, "register")) {
        ret_val = recv(all_connections_i, id, SIZE, 0);
        ret_val = recv(all_connections_i, pass, SIZE, 0);
        if(idPass_Check(id, pass, cmd)) status_val = send(all_connections_serving,"userfound\n", SIZE, 0);
	else {
            *userLoggedIn = 1;
            FILE *app = fopen("akun.txt", "a+");
            fprintf(app, "%s:%s\n", id, pass);
            fclose(app);
            status_val = send(all_connections_serving,"regloginsuccess\n", SIZE, 0);
        }
    }
    else if(!strcmp(cmd, "login")) {
        ret_val = recv(all_connections_i, id, SIZE, 0);
        ret_val = recv(all_connections_i, pass, SIZE, 0);
        if(!idPass_Check(id, pass, cmd)) status_val = send(all_connections_serving,"wrongpass\n", SIZE, 0);
        else {
            *userLoggedIn = 1;
            status_val = send(all_connections_serving,"regloginsuccess\n", SIZE, 0);
        }
    }
}
//Ae
int main () {
    fd_set read_fd_set;
    struct sockaddr_in new_addr;
    int server_fd, new_fd, i, serving = 1;
    int ret_val, ret_val1, ret_val2, ret_val3, ret_val4, status_val;
    char mssg[SIZE], id[SIZE], pass[SIZE], cmd[SIZE];
    socklen_t addrlen;
    char buf[DATA_BUFF];
    int all_connections[MAX_CONNECT];
	if(access("akun.txt", F_OK ) != 0 ) {
		FILE *fp = fopen("akun.txt", "w+");
		fclose(fp);
	}
        if(access("files.tsv", F_OK ) != 0 ) {
		FILE *fp = fopen("files.tsv", "w+");
		fclose(fp);
	}
    server_fd = make_tcp_server_socket();
    if (server_fd == -1) {
        fprintf(stderr, "Failed to create a server\n");
        return -1;
    }
    for (i=0;i < MAX_CONNECT;i++) {
        all_connections[i] = -1;
    }
    all_connections[0] = server_fd;

	printf("\nServer is running....\n\n");
    int userLoggedIn = 0;
    while (1) {
        FD_ZERO(&read_fd_set);
        for (i=0;i < MAX_CONNECT;i++) {
            if (all_connections[i] >= 0) {
                FD_SET(all_connections[i], &read_fd_set);
            }
        }
        ret_val = select(FD_SETSIZE, &read_fd_set, NULL, NULL, NULL);
        if (ret_val >= 0 ) {
            if (FD_ISSET(server_fd, &read_fd_set)) {
                new_fd = accept(server_fd, (struct sockaddr*)&new_addr, &addrlen);
                if (new_fd >= 0) {
                    printf("Accepted a new login try:\n\n");
                    for (i=0;i < MAX_CONNECT;i++) {
                        if (all_connections[i] < 0) {
                            all_connections[i] = new_fd;
                            if(i != serving) {
                                ret_val1 = send(all_connections[i], "wait",  SIZE, 0);
                            } else {
                                ret_val1 = send(all_connections[i], "serve",  SIZE, 0);
                            }
                            break;
                        }
                    }
                } else {
                    fprintf(stderr, "accept failed [%s]\n", strerror(errno));
                }
                ret_val--;
                if (!ret_val) continue;
            }
            for (i=1;i < MAX_CONNECT;i++) {
                if ((all_connections[i] > 0) &&
                    (FD_ISSET(all_connections[i], &read_fd_set))) {
                    ret_val3 = recv(all_connections[i], cmd, sizeof(cmd), 0);
                    printf("Returned fd is %d [index, i: %d]\n", all_connections[i], i);
                    printf("Command : %s\n", cmd);
                    if (ret_val1 == 0 || ret_val2 == 0 || ret_val3 == 0) {
                        printf("Id of the user now : %s\n", id);
                        printf("pass of the user now : %s\n", pass);
                        printf("Closing connection for fd:%d\n\n", all_connections[i]);
                        close(all_connections[i]);
                        all_connections[i] = -1;
                        while(1) {
                            if(serving == 9) {
                                serving = 1;
                                break;
                            }
                            if(all_connections[serving + 1] != -1) {
                                serving++;
                                break;
                            }
                            serving++;
                        }
                        userLoggedIn = 0;
                        if(all_connections[serving] != -1) status_val = send(all_connections[serving], "serve",  SIZE, 0);
                    }
                    if (ret_val3 > 0) {
                        if(!strcmp(cmd, "register") || !strcmp(cmd, "login"))
                              register_login(all_connections[i], cmd, id, pass, &userLoggedIn,
                                            all_connections[serving] );
                        printf("Id of the user now : %s\n", id);
                        printf("pass of the user now : %s\n\n", pass);
                    }
                    if (ret_val1 == -1 || ret_val2 == -1 || ret_val3 == -1) {
                        printf("recv() failed for fd: %d [%s]\n",all_connections[i], strerror(errno));
                        break;
                    }
                }
                ret_val1--;
                ret_val2--;
                ret_val3--;
                if (!ret_val1) continue;
                if (!ret_val2) continue;
                if (!ret_val3) continue;
            }
	}
    }
    for(i=0;i < MAX_CONNECT;i++) {
        if (all_connections[i] > 0) close(all_connections[i]);
    }

    return 0;
}
