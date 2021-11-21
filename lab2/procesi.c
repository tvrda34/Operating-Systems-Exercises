#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>

int brojProcesa, brojIteracija;
int *a;
int Id;

void proc(){
    for(int i=0; i<brojIteracija; i++){
        *a = *a + 1;
    }
}

int main(int argc, char *argv[]){
  
    if(argc != 3){
        printf("Potrebno je unijeti dva parameta 1. broj procesa, 2. broj iteracija\n");
        exit(1);
    }
    Id = shmget(IPC_PRIVATE, sizeof(int), 0600);
    if (Id == -1){
        printf("Nema zajedničke memorije\n");
        exit(1);
    }
    a = (int *) shmat(Id, NULL, 0);
    *a = 0;
    
    brojProcesa = atoi(argv[1]);
    brojIteracija = atoi(argv[2]);

    for(int i=0; i<brojProcesa; i++){
      if(fork() == 0){
          proc();
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