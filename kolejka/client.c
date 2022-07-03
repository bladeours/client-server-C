#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>


/*
 	man msgget
	man msgrcv <- prosty przyklad
	man msgctl
*/

struct Message{
        long type;
        char text[20];
}message;


int main(int argc, char* argv[]){
    int msgid = msgget((key_t)69,0666|IPC_CREAT);
	if(msgid == -1){
		printf("msgid error\n");
		return -1;
	}
	message.type = 1;
	strcpy(message.text,argv[1]);
	
	if(msgsnd(msgid,&message,20,0) == -1){
		printf("msgsnd error\n");
		return -1;
	}

	msgctl(msgid,IPC_RMID,0);
	return 0;
}
