#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

struct msg_buffer {
   long msg_type;
   int Mrow,Mcols;
   int multi[10][10];
} message;

int factorial(int n) { 
  if(n==0){
    n=1;
  }
 if (n>=1)
    return n*factorial(n-1);
  else
    return 1;
} 

int permutation(int n, int r){

    return factorial(n) / factorial(n-r);
}


void *solve(void* arg)
{
    int *data = (int *)arg;
    int element = 0,i=0;
    int x = data[0];
    for(i=1;i<=x;i++){
      if(data[i]>=data[i+x]){
        element = permutation(data[i],data[i+x]);
      }else if(data[i+x]>data[i]){
        element=factorial(data[i]);
      }else{
        element = 0;
      }
      printf("element:%d",element);
    }
    int *p = (int*)malloc(sizeof(int));
         *p = element;
    pthread_exit(p);
}

int main(int argc, char *argv[]){
   int matrixb[10][10],index = 1;
  
   key_t key = ftok("progfile", 65);
   int msgid = msgget(key, 0666 | IPC_CREAT);
   msgrcv(msgid, &message, sizeof(message), 1, 0);
   
    printf("input matrix B\n");
    for(int i=0; i<message.Mrow; i++) {
        for(int j=0; j<message.Mcols; j++){
            matrixb[i][j] = atoi(argv[index]);
            index++;
        }
    }
   printf("Matrix b: \n");
   for (int c = 0; c < message.Mrow; c++) {
      for (int d = 0; d < message.Mcols; d++)
        printf("%d\t", matrixb[c][d]);
      printf("\n");
    }
   printf("Matrix A from soal2a.c: \n");
   for (int c = 0; c < message.Mrow; c++) {
      for (int d = 0; d < message.Mcols; d++)
        printf("%d\t", message.multi[c][d]);
      printf("\n");
    }
  int max = message.Mrow*message.Mcols;
  pthread_t *threads;
  threads = (pthread_t*)malloc(max*sizeof(pthread_t));
  int count = 0;
  int* result = NULL;
  for (int i = 0; i < message.Mrow; i++)
    for (int j = 0; j < message.Mcols; j++){
      result = (int *)malloc((24)*sizeof(int));
      // segfault
        result[0] = 24;
        for (int k = 0; k < max; k++)
          result[k+1] = message.multi[i][k];
        for (int k = 0; k < max; k++)
          result[k+max+1] = matrixb[k][j];
      pthread_create(&threads[count++], NULL, solve, (void*)(result));
    }

  printf("Result : \n");
  for (int i = 0; i < max; i++) {
    void *k;
    pthread_join(threads[i], &k);
    int *p = (int *)k;
    printf("%d ",*p);
    if ((i + 1) % message.Mcols == 0)
        printf("\n");
  }



  msgctl(msgid, IPC_RMID, NULL);
  
}