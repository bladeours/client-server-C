#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

/*
	man 2 open
	man 2 write
*/

int main(int argc, char * argv[]){
	
	int file = open("fifo",O_WRONLY);
	char toServer[20];
	strcpy(toServer,argv[1]);
	write(file,toServer,20);
	close(file);
	return 0;
}
