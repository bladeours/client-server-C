/*
Serwer sekwencyjny oparty na semaforach i pamieci wspoldzielonej (wersja standardowa). Do synchronizacji dostepu do pamieci uzywane sa dwa semafory: 0 (wartosc 0, gdy klient nie ma dostepu do wspolnej pamieci i warotsc 1, gdy klient ma dostep do wspolnej pamieci), 1 (wartosc 0, gdy serwer nie ma dostepu do wspolnej pamieci i wartosc 1, gdy serwer ma dostep do wspolnej pamieci).
*/

#include<stdio.h>
#include<string.h>
#include<sys/sem.h>
#include<sys/shm.h>
#include<stdlib.h>
#include<signal.h>
#include<unistd.h>
#include<fcntl.h>
#include<errno.h>
#include<sys/types.h>

typedef struct sembuf bufor_semaforow;
int id_semafor;
int id_pamieci;
char *pamiec_wspolna;
char *sciezka;
int czy_skasowac = 1;

void al(int i) {
    if (i == SIGTERM || i == SIGINT) {
        printf("Serwer: Koniec pracy serwera.\n");
        shmdt(pamiec_wspolna);
        shmctl(id_pamieci, IPC_RMID, NULL);
        semctl(id_semafor, 0, IPC_RMID, NULL);
        if (czy_skasowac) {
            unlink(sciezka);
        }
        free(sciezka);
        exit(0);
    }
}

int main(int argc, char *argv[]) {
    key_t klucz_semafory, klucz_pamiec_wspolna;
    bufor_semaforow ustaw_semafory;
    int plik = 0;

    signal(SIGTERM, al);
    signal(SIGINT, al);
    if (argc > 1) {
        sciezka = strndup(argv[1], strlen(argv[1]));
    } else {
        sciezka = strndup("/tmp/roboczy.sem_serwer", strlen("/tmp/roboczy.sem_serwer"));
    }
    if (access(sciezka, F_OK) == 0) {
        czy_skasowac = 0;
    }
    if (czy_skasowac && ((plik = open(sciezka, O_CREAT | O_EXCL, 0600)) == -1)) {
        fprintf(stderr, "Serwer: Blad utworzenia pliku %s: %s.\n", sciezka, strerror(errno));
        free(sciezka);
        return -1;
    } else {
        close(plik);
        if ((klucz_semafory = ftok(sciezka, 0)) == -1 || (klucz_pamiec_wspolna = ftok(sciezka, 1)) == -1) {
            perror("Serwer: Blad funkcji ftok.\n");
            if (czy_skasowac) {
                unlink(sciezka);
            }
            free(sciezka);
            return -1;
        } else {
            if ((id_pamieci = shmget(klucz_pamiec_wspolna, 50 * sizeof(char), 0600 | IPC_CREAT | IPC_EXCL)) == -1) {
                perror("Serwer: Blad funkcji shmget.\n");
                if (czy_skasowac) {
                    unlink(sciezka);
                }
                free(sciezka);
                return -1;
            } else {
                pamiec_wspolna = shmat(id_pamieci, (char *) 0, 0);
                if ((id_semafor = semget(klucz_semafory, 2, 0600 | IPC_CREAT | IPC_EXCL)) == -1) {
                    perror("Serwer: Blad funkcji semget.\n");
                    shmdt(pamiec_wspolna);
                    shmctl(id_pamieci, IPC_RMID, NULL);
                    if (czy_skasowac) {
                        unlink(sciezka);
                    }
                    free(sciezka);
                    return -1;
                } else {
                    u_short wartosci_poczatkowe_semaforow[] = {1, 0};

                    if ((semctl(id_semafor, 0, SETALL, wartosci_poczatkowe_semaforow)) == -1) {
                        perror("Serwer: Blad funkcji semctl.\n");
                        semctl(id_semafor, 0, IPC_RMID, NULL);
                        shmdt(pamiec_wspolna);
                        shmctl(id_pamieci, IPC_RMID, NULL);
                        if (czy_skasowac) {
                            unlink(sciezka);
                        }
                        free(sciezka);
                        return -1;
                    } else {
                        for (;;) {
                            ustaw_semafory.sem_num = 1;
                            ustaw_semafory.sem_op = -1;
                            ustaw_semafory.sem_flg = 0;
                            printf("Serwer: Oczekuje na wiadomosc od klienta.\n");
                            if (semop(id_semafor, &ustaw_semafory, 1) == -1) {
                                perror("Server: Blad oczekiwania na otwarcie semafora do odczytu dla serwera (oczekiwanie serwera), funkcja semop.\n");
                                shmdt(pamiec_wspolna);
                                shmctl(id_pamieci, IPC_RMID, NULL);
                                semctl(id_semafor, 0, IPC_RMID, NULL);
                                if (czy_skasowac) {
                                    unlink(sciezka);
                                }
                                free(sciezka);
                                return -1;
                            } else {
                                char *wiadomosc = strdup(pamiec_wspolna);

                                ustaw_semafory.sem_num = 0;
                                ustaw_semafory.sem_op = 1;
                                ustaw_semafory.sem_flg = 0;
                                if (semop(id_semafor, &ustaw_semafory, 1) == -1) {
                                    free(wiadomosc);
                                    perror("Serwer: Blad oczekiwania na otwarcie semafora do zapisu dla klienta (zwolnienie dla klienta), funkcja semop.\n");
                                    shmdt(pamiec_wspolna);
                                    shmctl(id_pamieci, IPC_RMID, NULL);
                                    semctl(id_semafor, 0, IPC_RMID, NULL);
                                    if (czy_skasowac) {
                                        unlink(sciezka);
                                    }
                                    free(sciezka);
                                    return -1;
                                } else {
                                    printf("Serwer: Wiadomosc od klienta: %s.\n", wiadomosc);
                                }
                                free(wiadomosc);
                            }
                        }
                    }
                }
            }
        }
    }
}