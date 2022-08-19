#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int file;

void signalHandler(int sig){
	printf("server stop\n");
	close(file);
	exit(0);
}

int main(){
	signal(SIGINT, signalHandler);
	if(mkfifo("fifo",0600|O_CREAT) == -1){
		perror("mkfifo");
	}
	char fromClient[50];
	while(1){

		file = open("fifo",O_RDONLY);
		if(file == -1){
			perror("open");
		}
		if(read(file, fromClient, 50) == -1){
			perror("read");
		}
		printf("from client: %s\n",fromClient);
		close(file);
		sleep(1);
	}

	
}
