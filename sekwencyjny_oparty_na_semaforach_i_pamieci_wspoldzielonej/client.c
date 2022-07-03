#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>

/*
PRZYDATNE MANUALE
man sem_post
man sem_wait <- jakis przyklad ale biedny
man sem_open
man ftok
man shmget
man sem_close
*/


//trzeba kompilowac z -pthread

int main(int argc, char *argv[]){

    //tworzenie semaforow
    sem_t *clientSemaphore = sem_open("clientSemaphore", 0);
    if(clientSemaphore == NULL){
        printf("problem z otwarciem semaforu client");
        return -1;
    }

    sem_t *serverSemaphore = sem_open("serverSemaphore", 1);
    if(serverSemaphore == NULL){
        printf("problem z otwarciem semaforu consumer");
        return -1;
    }
    //tworzenie klucza do bloku pamieci
    key_t key;
    key = ftok("blok_pamieci",0);
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


    printf("czekam na pozwolenie na zapis\n");
    sem_wait(serverSemaphore); //czekam na semafor 
    printf("Writing: %s\n",argv[1]);
    strncpy(block,argv[1],512);
    sem_post(clientSemaphore);
    
    //zamykam semafory
    sem_close(clientSemaphore);
    sem_close(serverSemaphore);
    return 0;
}