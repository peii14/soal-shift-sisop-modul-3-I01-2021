#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>

struct mesg_buffer {
    long msg_type;
    int Mrow,Mcols;
    int multi[10][10];
} message;

int main(int argc, char *argv[])
{
    int row1 =4, cols1=3,row2=3,cols2=6, sum=0; 
    int m[10][10], m2[10][10];  
    int index =1, index2 = 13; 
    
    key_t key = ftok("progfile", 65);
    int msgid;
    msgid = msgget(key, 0666 | IPC_CREAT);
    message.Mrow = row1;
    message.Mcols = cols2;
    message.msg_type = 1;

    printf("input matrix 1\n");
    for(int i=0; i<row1; i++) {
        for(int j=0; j<cols1; j++){
            m[i][j] = atoi(argv[index]);
            index++;
        }
    }
    for (int c = 0; c < row1; c++) {
      for (int d = 0; d < cols1; d++)
        printf("%d\t", m[c][d]);
 
      printf("\n");
    }
    printf("input matrix 2\n");
     for(int i=0; i<row2; i++) {
        for(int j=0; j<cols2; j++){
            m2[i][j] = atoi(argv[index2]);
            index2++;
        }
     }
     for (int c = 0; c < row2; c++) {
      for (int d = 0; d < cols2; d++)
        printf("%d\t", m2[c][d]);

      printf("\n");
    }
 
    for (int c = 0; c < row1; c++) {
      for (int d = 0; d < cols2; d++) {
        for (int k = 0; k < row2; k++) {
          sum = sum + m[c][k]*m2[k][d];
        }
        message.multi[c][d] = sum;
        sum = 0;
      }
    }
    msgsnd(msgid, &message, sizeof(message), 0);
    printf("SENT\n");
    
    return 0;
}

    