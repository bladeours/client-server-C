#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

/*q
	man 3 mkfifo
	man 2 open
	man 2 write
*/

int main(){
	//tworzymy kolejke (plik) "fifo"
	mkfifo("fifo",0600);
		
	while(1){
		//otwieramy go
		int file = open("fifo",O_RDONLY);
		char fromClient[20];
		read(file, fromClient, 20);
		printf("from client: %s\n",fromClient);
		close(file);
	}
	return 0;
}
