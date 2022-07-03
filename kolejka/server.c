#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>

/*
 	man msgget
	man msgrcv <- prosty przyklad
*/


struct Message{
	long type;
	char text[20];
}message;


int main(){

	while(1){
		int msgid = msgget((key_t)69,0666|IPC_CREAT);
		if(msgid == -1){
			printf("msgid error\n");
			return -1;
		}
		if(msgrcv(msgid,(void *)&message,20,0,0) == -1){
			printf("msgrcv error\n");
			return -1;
		}

		printf("%s\n",message.text);
	}
	

	return 0;
}
