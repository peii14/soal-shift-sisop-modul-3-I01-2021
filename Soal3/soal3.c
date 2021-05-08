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
            printf("error\n");
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
            printf("File type = %s\nBerhasil Dikategorikan\n",lowcase);
            mkdir(temp, 0777);
        }
    }
    //klo file gaada tipe file nya
    else
    {
        strcpy(temp,cwd);
        strcat(temp,"/");
        strcat(temp,"Unknown");
        printf("File type = Unknown\nBerhasil Dikategorikan\n");
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
    if(getcwd(cwd, sizeof(cwd)) != NULL) 
    {
        printf("Current working dir: %s\n", cwd);
    } 

    int i=0;
    //loop sebanyak argumen file trus masuk ke thread yg dibuat buat mindahin file
    if (strcmp(argv[1],"-f") == 0) 
    {
        for(int j = 2 ; j < argc ; j++ )
        {
            pthread_create(&(tid[i]),NULL,function,argv[j]);
            pthread_join(tid[i],NULL);
            i++;
        }
    }
}