#define _XOPEN_SOURCE
#define _XOPEN_SOURCE_EXTENDED 

#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#define TIME_INTERVAL_MAX 5
#define TIME_INTERVAL_MIN 3
#define SIGNAL_NUMBER 4

int pid=0;

void prekidna_rutina(int sig){
   kill(pid, SIGKILL);
   exit(0);
}

int main(int argc, char *argv[]){
   pid=atoi(argv[1]);
   sigset(SIGINT, prekidna_rutina);
   srand(time(NULL));

   while(1){
       int randomTime = rand() % (TIME_INTERVAL_MAX - TIME_INTERVAL_MIN +1) + TIME_INTERVAL_MIN;
       sleep(randomTime);
       int randomSignal = rand() % SIGNAL_NUMBER;
       switch(randomSignal){
           case 1:
             kill(pid, SIGUSR1);
             break;
           case 2:
             kill(pid, SIGUSR2);
             break;
           case 3:
             kill(pid, SIGALRM);
             break;
            case 4:
             kill(pid, SIGTERM);
             break;
       }
   }
   return 0;
}