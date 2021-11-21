#define _XOPEN_SOURCE
#define _XOPEN_SOURCE_EXTENDED

#include <stdio.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <time.h>

sem_t *KO;
sem_t *bozicnjak;
sem_t *konzultacije;
sem_t *sobovi;
int *br_sobova, *br_patuljaka;

pid_t pid_pol, pid_bozicnjak;

void patuljak () {
    printf("Patuljak %d aktivan\n", *br_patuljaka + 1);
    sem_wait(KO);
    *br_patuljaka = *br_patuljaka + 1;
    
    if(*br_patuljaka == 3){
        sem_post(bozicnjak);
    }
    sem_post(KO);
    sem_wait(konzultacije);

	exit(0);
}

void sob() {
    printf("Sob broj %d se vraca s godisnjeg odmora\n", *br_sobova+1);
    sem_wait(KO);
    *br_sobova = *br_sobova + 1;
     if(*br_sobova == 10){
        sem_post(bozicnjak);
    }
    sem_post(KO);
    sem_wait(sobovi);

    exit(0);
}

void sjeverni_pol () {
	printf("Proces Sjeverni pol zapocinje s radom\n");
    srand ((unsigned int) time(NULL));
	while (1) {
        int cekanje = rand() % 3 + 1; //cekaj sluc. vrijeme 1-3 sec
        sleep(cekanje);

        if(rand() % 2 == 1 && *br_sobova < 10){ // &&nisu se svi vratili uvjet
            if(fork() == 0){
                sob();
                exit(0);
            }   
        }
        if(rand() % 2 == 1){
             if (fork()==0) {
                patuljak(); 
                exit(0);
            }
        }
	}
	exit(0);
}

void djed_bozicnjak () {
	while (1) {
        printf("Djed bozicnjak spava\n");
		sem_wait(bozicnjak);
        sem_wait(KO);

        if(*br_sobova == 10 && *br_patuljaka > 0){
            sem_post(KO);
            printf("Djed bozicnjak ukrcava poklone i raznosi ih\n");
            sleep(2);
            sem_wait(KO);

            for(int i=0; i<10; i++){
                sem_post(sobovi);
            }
            printf("Djed bozicnjak poslao je sve sobove na zasluzeni odmor\n");
            *br_sobova = 0;
        }

        if(*br_sobova == 10){ //svi pred vratima
            sem_post(KO);
            printf("Djed bozicnjak se budi i hrani sobove\n");
            sem_wait(KO);
        }

        if(*br_patuljaka >= 3){
            sem_post(KO);
            printf("Djed bozicnjak se budi i rjesava problem patuljaka\n");
            sleep(2);

            for(int i=0; i<3; i++){
                sem_post(konzultacije);
            }
            *br_patuljaka -= 3;
        }
        sem_post(KO);	
	}
	exit(0);
}

void zavrsi(int sig) {
    printf("Unistavanje semafora i zajednicke memorije\n");
    sem_destroy(KO);
    sem_destroy(konzultacije);
    sem_destroy(bozicnjak);
    sem_destroy(sobovi);
	shmdt(KO);

    printf("Kraj gasi procese sj. pol i djed bozicnjak\n");
    kill(pid_pol, SIGKILL);
    kill(pid_bozicnjak, SIGKILL);

    exit(0);
}


int main () {
	int i, ID;

	ID = shmget (IPC_PRIVATE, 4 * sizeof(sem_t) + (2 * sizeof(int)), 0600);
	KO = shmat(ID, NULL, 0);
	shmctl(ID, IPC_RMID, NULL); //moze odmah ovdje, nakon shmat

    //zajednicki spremnik
    bozicnjak = (sem_t *) (KO +1);
    konzultacije = (sem_t *) (bozicnjak +1);
    sobovi = (sem_t *) (konzultacije +1);
	br_patuljaka = (int *) (sobovi + 1);
    br_sobova = (int *) (br_patuljaka + 1);

    //incijalizacija semafora
	sem_init(KO, 1, 1);
    sem_init(bozicnjak, 1, 0);
    sem_init(konzultacije, 1, 0);
    sem_init(sobovi, 1, 0);

    //zajednicke varijable
    *br_patuljaka = *br_sobova = 0;

    pid_pol = fork();
	if ( pid_pol == 0 ) {
		sjeverni_pol();
        exit(0);
	}
	else if (pid_pol == -1) {
		perror("Greska pri stvaranju procesa sjeverni pol\n");
		exit(1);
	}

    pid_bozicnjak= fork();
	if ( pid_bozicnjak == 0 ) {
		djed_bozicnjak();
        exit(0);
	}
	else if (pid_bozicnjak == -1) {
		perror("Greska pri stvaranju procesa djeda bozicnjaka\n");
		exit(1);
	}

    sigset(SIGINT, zavrsi);

    (void) wait(NULL);
    (void) wait(NULL);
    zavrsi(0);

	return 0;
}