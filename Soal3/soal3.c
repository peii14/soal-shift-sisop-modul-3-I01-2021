#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <syslog.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <dirent.h>
#include <string.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>
#include <ctype.h>
#include <unistd.h>

char cwd[100];
pthread_t tid[3];
char *arr[4],*arr2[20],arr3[100], category[100];
int n = 0, m = 0;

void* function(void *arg)
{
    strcpy(category,arg);
    char *token,*token1;
	pthread_t id=pthread_self();

    //kategoriin filenya, dapet nama ama tipe file nya
    token1 = strtok(category, "/");
    while( token1 != NULL ) 
    {
        arr2[m] = token1;
        m++;
        token1 = strtok(NULL, "/");
    }
    strcpy(arr3,arr2[m-1]);
    
    //ngambil tipe file disini
    token = strtok(arr2[m-1], ".");
    while( token != NULL ) 
    {
        arr[n] = token;
        n++;
        token = strtok(NULL, ".");
    }

    //disini ngubah tipe file nya klo kapital jadi huruf kecil semua
    char lowcase[100];
    strcpy(lowcase,arr[n-1]);
    for(int i = 0; lowcase[i]; i++)
    {
        lowcase[i] = tolower(lowcase[i]);
    }
  
    DIR *directory;
    struct dirent *entry;
    char temp[100];
    directory = opendir(cwd);
    int checking = 0;
    if( n > 1 )
    {
        if(directory == NULL)
        {
            printf("Sad, gagal :(\n");
        }

        //ngecek nama tipe file nya udh ada blom
        while((entry=readdir(directory)))
        {
            if(strcmp(entry->d_name,lowcase) == 0 && entry->d_type == 4)
            {
                checking = 1;
                break;
            }
        }
        //klo belom kesini
        if(checking == 0)
        {
            strcpy(temp,cwd);
            strcat(temp,"/");
            strcat(temp,lowcase);
            mkdir(temp, 0777);
        }
    }
    //klo file gaada tipe file nya
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
}

int main(int argc, char *argv[]) 
{
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
}
