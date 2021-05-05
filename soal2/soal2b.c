#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct msg_buffer {
   long msg_type;
   int Mrow,Mcols;
   int multi[10][10];
} message;

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
   msgctl(msgid, IPC_RMID, NULL);
  
}