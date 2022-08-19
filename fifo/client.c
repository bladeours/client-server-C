#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[] ){
	int file = open("fifo",O_WRONLY);
	if(file == -1){
		perror("open");
	}
	if(write(file, argv[1], 50) == -1){
		perror("write");
	}
	close(file);
	return 0;
}
