/*
Klient oparty na semaforach i pamieci wspoldzielonej dla serwera sekwencyjnego (wersja standardowa). Do synchronizacji dostepu do pamieci uzywane sa dwa semafory: 0 (wartosc 0, gdy klient nie ma dostepu do wspolnej pamieci i warotsc 1, gdy klient ma dostep do wspolnej pamieci), 1 (wartosc 0, gdy serwer nie ma dostepu do wspolnej pamieci i wartosc 1, gdy serwer ma dostep do wspolnej pamieci).
*/

#include<sys/sem.h>
#include<sys/shm.h>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

typedef struct sembuf bufor_semaforow;

int main(int argc, char *argv[]) {
    key_t klucz_semafory, klucz_pamiec_wspolna;
    bufor_semaforow ustaw_semafory;
    int id_semafor;
    int id_pamieci;
    char *sciezka;
    int czy_czekac = 0, liczba_sekund;
    pid_t pid = getpid();

    if (argc > 1) {
        czy_czekac = 1;
        liczba_sekund = strtol(argv[1], NULL, 10);
    }
    if (argc > 2) {
        sciezka = strndup(argv[2], strlen(argv[2]));
    } else {
        sciezka = strndup("/tmp/roboczy.sem_serwer", strlen("/tmp/roboczy.sem_serwer"));
    }
    if ((klucz_semafory = ftok(sciezka, 0)) == -1 || (klucz_pamiec_wspolna = ftok(sciezka, 1)) == -1) {
        perror("Klient: Blad funkcji ftok.\n");
        free(sciezka);
        return -1;
    } else {
        free(sciezka);
        if ((id_semafor = semget(klucz_semafory, 2, 0600)) == -1) {
            perror("Klient: Blad funkcji semget.\n");
            return -1;
        } else {
            ustaw_semafory.sem_num = 0;
            ustaw_semafory.sem_op = -1;
            ustaw_semafory.sem_flg = 0;
            printf("Klient: Moj numer procesu to %d. Oczekuje na mozliwosc zapisu.\n", pid);
            if (semop(id_semafor, &ustaw_semafory, 1) == -1) {
                perror("Klient: Blad oczekiwania na otwarcie semafora do zapisu dla klienta (oczekiwanie klienta), funkcja semop.\n");
                return -1;
            } else {
                if ((id_pamieci = shmget(klucz_pamiec_wspolna, 50 * sizeof(char), 0600)) == -1) {
                    perror("Klient: Blad funkcji shmget.\n");
                    return -1;
                } else {
                    char *wiadomosc = (char *) malloc(100 * sizeof(char));
                    char *pamiec_wspolna = shmat(id_pamieci, (char *) 0, 0);
                    
                    sprintf(wiadomosc, "Moj numer procesu to %d", pid);
                    strcpy(pamiec_wspolna, wiadomosc);
                    printf("Klient: Wiadomosc zapisana.\n");
                    free(wiadomosc);
                    shmdt(pamiec_wspolna);
                    if (czy_czekac) {
                        printf("Klient: Oczekuje %d sekund na zwolnienie semafora do czytania dla serwera.\n", liczba_sekund);
                        sleep(liczba_sekund);
                    }
                    ustaw_semafory.sem_num = 1;
                    ustaw_semafory.sem_op = 1;
                    ustaw_semafory.sem_flg = 0;
                    if (semop(id_semafor, &ustaw_semafory, 1) < 0) {
                        perror("Klient: Blad oczekiwania na otwarcie semafora do odczytu dla serwera (zwolnienie dla serwera).\n");
                        return -1;
                    }
                    return 0;
                }
            }
        }
    }
}