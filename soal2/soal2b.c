#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>

struct mesg_buffer {
   long msg_type;
   int Mrow,Mcols;
   int multi[10][10];
} message;

int main(){
   key_t key = ftok("progfile", 65);
   int msgid = msgget(key, 0666 | IPC_CREAT);
   msgrcv(msgid, &message, sizeof(message), 1, 0);
   
   for (int c = 0; c < message.Mrow; c++) {
      for (int d = 0; d < message.Mcols; d++)
        printf("%d\t", message.multi[c][d]);
      printf("\n");
    }
   msgctl(msgid, IPC_RMID, NULL);
  
}