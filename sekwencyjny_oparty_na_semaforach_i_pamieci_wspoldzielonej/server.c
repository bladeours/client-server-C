#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/stat.h> 

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

    //tworzenie semaforów

    sem_t *clientSemaphore = sem_open("clientSemaphore",O_CREAT, 0660, 0);
    if(clientSemaphore == NULL){
        printf("problem z otwarciem semaforu client");
        return -1;
    }

    sem_t *serverSemaphore = sem_open("serverSemaphore",O_CREAT, 0660, 1);
    if(serverSemaphore == NULL){
        printf("problem z otwarciem semaforu server");
        return -1;
    }


    //tworzenie klucza do bloku pamieci (trzeba stworzyc fizycznie plik "blok_pamieci")
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

    while(1)
    {
        // printf("czekam na wiadomosc\n");
        sem_wait(clientSemaphore);
        if(strlen(block) > 0){
            printf("reading: %s\n",block);
            block[0]=0;
        }
        sem_post(serverSemaphore);
    }
    
    return 0;
}