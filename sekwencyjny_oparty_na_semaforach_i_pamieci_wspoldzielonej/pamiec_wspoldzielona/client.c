#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>


int main(int argc, char *argv[]){

    
    //tworzenie klucza do bloku pamieci
    key_t key;
    key = ftok("plik",0);
    if (key == -1){
        printf("nie mozna stworzyc klucza");
        return -1;
    }
    
    //pobieranie id wspoldzielonego bloku pamieci
    int sharedBlockId =  shmget(key,512,0644 | IPC_CREAT);
    if(sharedBlockId == -1 ){
        printf("pobranie wspoldzielonego bloku pamieci nie dziala");
        return -1;
    }

    //pobranie wlasciwego bloku pamieci
    char *block = shmat(sharedBlockId,NULL,0);
    if(block == NULL){
        printf("problem z pobraniem bloku");
    }

    printf("Writing: %s\n",argv[1]);
    strncpy(block,argv[1],512);
}