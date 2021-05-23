# soal-shift-sisop-modul-3-I01-2021

Keverk adalah orang yang cukup ambisius dan terkenal di angkatannya. Sebelum dia menjadi ketua departemen di HMTC, dia pernah mengerjakan suatu proyek dimana keverk tersebut meminta untuk membuat server database buku. Proyek ini diminta agar dapat digunakan oleh pemilik aplikasi dan diharapkan bantuannya dari pengguna aplikasi ini. 

Di dalam proyek itu, Keverk diminta: 

a. Pada saat client tersambung dengan server, terdapat dua pilihan pertama, yaitu register dan login. Jika memilih register, client akan diminta input id dan passwordnya untuk dikirimkan ke server. User juga dapat melakukan login. Login berhasil jika id dan password yang dikirim dari aplikasi client sesuai dengan list akun yang ada didalam aplikasi server. Sistem ini juga dapat menerima multi-connections. Koneksi terhitung ketika aplikasi client tersambung dengan server. Jika terdapat 2 koneksi atau lebih maka harus menunggu sampai client pertama keluar untuk bisa melakukan login dan mengakses aplikasinya. Keverk menginginkan lokasi penyimpanan id dan password pada file bernama akun.txt dengan format :

akun.txt
id:password
id2:password2

server.c

```shell
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
```
client.c

```shell
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
```

b. Sistem memiliki sebuah database yang bernama files.tsv. Isi dari files.tsv ini adalah path file saat berada di server, publisher, dan tahun publikasi. Setiap penambahan dan penghapusan file pada folder file yang bernama  FILES pada server akan memengaruhi isi dari files.tsv. Folder FILES otomatis dibuat saat server dijalankan. 

```shell
        if(access("files.tsv", F_OK ) != 0 ) {
		FILE *fp = fopen("files.tsv", "w+");
		fclose(fp);
	}
```

2. Crypto (you) is Loba's friend. One morning, Crypto saw Loba who was overwhelmed with his boss's assignment. Since Crypto is such a challenging person, he wanted to help Loba with his job. The details of the assignment are:
Create a matrix multiplication program (4x3 and 3x6) and then display the results. The matrix will contain the numbers 1-20 (no need to create a number filter).
Create a program using the output matrix of the previous program (program soal2a.c) (Note!: Use shared memory). Then the matrix will be calculated with the new matrix. As follows an example of calculation for the existing matrix. The calculation is that each cell originating from matrix A becomes a number for factorial, then cells from matrix B become the maximum factorial limit (from largest to smallest) (Note!: Use threads for calculations in each cell). 

		Provisions
		
If a >= b  -> a!/(a-b)!
If b > a -> a!
If 0 -> 0


Example :

A
B
Factorial numbers
4
4
4 3 2 1
4
5
4 3 2 1
4
3
4 3 2
4
0
0
0
4
0
4
6
4 3 2 1

		
		Example :

Matrix A
Matrix B
Result Matrix
0
4
0
4
0
4*3*2*1
4
5
6
2
4*3*2*1
5*4
5
6
6
0
5*4*3*2*1
0


For fear of lags in the process of helping Loba, Crypto also created a program (soal2c.c) to check the top 5 processes consuming computer resources with the command  “ps aux | sort -nrk 3,3 | head -5” (Note !: You must use IPC Pipes)

	Notes:
All matrices come from the input to the program.
Prohibited from using system ()


3. During his time of inactivity, a student named Alex. He had an idea to tidy up a number of files on his laptop. Because there are too many files, Alex asked Ayub for advice. Ayub suggests creating a C program to categorize the files. This program will move files according to their extension into a folder according to the extension. The folder is in the working directory in which the categorization program is run. 

Example:
# The category program is located at /home/izone/kategori
$ ./kategori -f path/to/file1.jpg path/to/file2.c path/to/file3.zip
#Result
/home/izone
|-jpg
|--file1.jpg
|-c
|--file2.c
|-zip
|--file3.zip


a. Program accepts -f option like stated above, with this option the user may add file arguments to be categorized as much as they want. 
Output:
File 1 : Berhasil Dikategorikan (jika berhasil)
File 2 : Sad, gagal :( (jika gagal)
File 3 : Berhasil Dikategorikan

    getcwd(cwd, sizeof(cwd));

    int i=0;
    //loop sebanyak argumen file trus masuk ke thread yg dibuat buat mindahin file
    if (strcmp(argv[1],"-f") == 0) 
    {
        for(int j = 2 ; j < argc ; j++ )
        {
            pthread_create(&(tid[i]),NULL,function,argv[j]);
            pthread_join(tid[i],NULL);
            printf("File %d : Berhasil Dikategorikan\n", i+1);
            i++;
        }
    }


b. Program may accept -d option to categorize a directory instead. With this option, the user may only input 1 directory as it's arguments, unlike the -f option where the user may input multiple file arguments. Example:
$ ./kategori -d /path/to/directory/

The command above will categorize the files in /path/to/directory, the categorization result will be saved in the working directory where the C program is called (categorization result is not located at /path/to/directory).
Output:
Jika berhasil, print “Direktori sukses disimpan!”
Jika gagal, print “Yah, gagal disimpan :(“

    else if (strcmp(argv[1],"-d") == 0 && argc == 3) 
    {
        i = 0;
        DIR *fd;
        struct dirent *masuk;
        char tempata[100];
        fd = opendir(argv[2]);
        //ngecek bisa dibuka apa engga direktorinya
        if(fd == NULL)
        {
            printf("Yah, gagal disimpan :(\n");
        }
        //ngebaca apa aja yg ada di direktorinya
        while( (masuk=readdir(fd)) )
        {
            if ( !strcmp(masuk->d_name, ".") || !strcmp(masuk->d_name, "..") )
            continue;

            //nyimpen path file yg dikategoriin di tempata
            strcpy(tempata,argv[2]);
            strcat(tempata,"/");
            strcat(tempata,masuk->d_name);
            //ngecek file bukan kalo iya masuk ke thread
            if(masuk->d_type == 8)
            {
            pthread_create(&(tid[i]),NULL,function,tempata); //bikin thread
            pthread_join(tid[i],NULL);
            i++;
            }
        }
        printf("Direktori Sukses Disimpan!\n");
    }


c. Other than the above options, this program accepts the * option. Example:
$ ./kategori \*

This option categorizes all the file in the working directory when the C program is run

    else if (strcmp(argv[1],"*") == 0 && argc == 2) 
    {
        i = 0;
        DIR *fd;
        struct dirent *masuk;
        char tempata[100];
        fd = opendir(cwd);

        if(fd == NULL)
        {
            printf("Yah, gagal disimpan :(\n");
        }
        char tempatsoal[100] = "/home/damdum/Soal3/soal3.c", tempatsoal3[100] = "/home/damdum/Soal3/soal3";
        while((masuk=readdir(fd)))
        {
            if (!strcmp(masuk->d_name, ".") || !strcmp(masuk->d_name, "..") )
            continue;
            
            strcpy(tempata,cwd);
            strcat(tempata,"/");
            strcat(tempata,masuk->d_name);
            //printf("tempata = %s\n", tempata);
            //printf("tempatsoal = %s\n", tempatsoal);
            //printf("tempatsoal3 = %s\n", tempatsoal3);
            if((strcmp(tempata,tempatsoal) == 0) || strcmp(tempata,tempatsoal3) == 0) 
            continue;
            
            else if(masuk->d_type == 8){
            pthread_create(&(tid[i]),NULL,function,tempata); //membuat thread
            pthread_join(tid[i],NULL);
            i++;
            }
        }
        printf("Direktori suskses disimpan!\n");
    }

d. All files must be moved into a folder. Files without extensions will be moved to a folder named "Unknown". Hidden files will be moved to a folder named "Hidden".

    else
    {
        strcpy(temp,cwd);
        strcat(temp,"/");
        strcat(temp,"Unknown");
        mkdir(temp, 0777);
    }
	

    char path[1024], destination[1024];
    strcpy(path,arg);
    strcpy(destination,cwd);
    strcat(destination,"/");
    if(n == 1 )
    {
        strcat(destination,"Unknown");
    }
    else
    {
        strcat(destination,lowcase);
    }
    strcat(destination,"/");
    strcat(destination,arr3);
    rename(path,destination);
    n = 0;
    m = 0;

	return NULL;

e. Each file to be categorized will be processed by a thread to make the program run in parallel to make it quicker.

But because Ayub is simply an idea man and doesn't want to help build the program, Ayub is asking your help to create the program.

Note: 
Category folder is not created manually, it must be created with the C program
This program is case insensitive. Example: JPG and jpg is treated the same
If the extension consists of more than one (example “.tar.gz”) the file will be put in a folder with the formermost dot character (example “tar.gz”)
Fork-exec and system() functions are not allowed
Code for section b and c must be recursive in design
