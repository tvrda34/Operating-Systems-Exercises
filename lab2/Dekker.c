#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdatomic.h>

int brojProcesa = 2, brojIteracija;
int Id;
atomic_int *a, *PRAVO, *ZASTAVICA;

void izadi_iz_kriticnog_odsjecka(int i, int j){
    *PRAVO = j;
    ZASTAVICA[i] = 0;
}

void udi_u_kriticni_odsjecak(int i, int j){
    ZASTAVICA[i] = 1;
    while(ZASTAVICA[j] != 0){
        if(*PRAVO == j){
            ZASTAVICA[i]=0;
            while(*PRAVO == j);
        }
        ZASTAVICA[i] = 1;
    }
}

void proc(int d){
    for(int i=0; i<brojIteracija; i++){
        udi_u_kriticni_odsjecak(d, 1-d);
        *a = *a + 1;
        izadi_iz_kriticnog_odsjecka(d, 1-d);
    }
}

int main(int argc, char *argv[]){
  
    if(argc != 2){
        printf("Potrebno je unijeti jedan parametar broj iteracija\n");
        exit(1);
    }
    Id = shmget(IPC_PRIVATE, sizeof(atomic_int) * (brojProcesa + 2), 0600);
    if (Id == -1){
        printf("Nema zajedničke memorije\n");
        exit(1);
    }

    a = (atomic_int *) shmat(Id, NULL, 0);
    PRAVO = a + 1;
    ZASTAVICA = PRAVO + brojProcesa;
    *a = *PRAVO = 0;

    brojIteracija = atoi(argv[1]);

    for(int i=0; i<brojProcesa; i++){
      if(fork() == 0){
          proc(i);
          exit(0);
      }
    }
    for(int i=0; i<brojProcesa; i++){
        wait(NULL);
    }

    printf("A=%d\n", *a);

    shmdt((char *) a);
    shmctl(Id, IPC_RMID, NULL);
    
    return 0;
}