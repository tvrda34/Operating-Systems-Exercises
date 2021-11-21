#define _XOPEN_SOURCE
#define _XOPEN_SOURCE_EXTENDED 

#include <stdio.h>
#include <signal.h>
#include <unistd.h>

#define N 6   
 
static int OZNAKA_CEKANJA[N];
int PRIORITET[N];
int TEKUCI_PRIORITET;

int sig[]={SIGUSR1, SIGUSR2, SIGALRM, SIGTERM, SIGINT};

void zabrani_prekidanje(){
   int i;
   for(i=0; i<5; i++)
      sighold(sig[i]);
}

void dozvoli_prekidanje(){
   int i;
   for(i=0; i<5; i++)
      sigrelse(sig[i]);
}
 
void obrada_prekida(int i){
  for(int j=0; j<i; j++){
      printf("- ");
  }
  printf("P ");
  for(int j=i; j<N-1;j++){
      printf("- ");
  }
  printf("\n");

  for(int j=0; j<5; j++){
    for(int k=0; k<i; k++){
        printf("- ");
    }
    printf("%d ", j+1);
    for(int k=i; k<N-1;k++){
        printf("- ");
    }
    printf("\n");
    if(j != 4)
        sleep(1);
  }

  for(int j=0; j<i; j++){
    printf("- ");
  }
  printf("K ");
  for(int j=i; j<N-1;j++){
      printf("- ");
  }
  printf("\n");
  
}

void prekidna_rutina(int sig){
   int n=-1;
   zabrani_prekidanje();
   switch(sig){
      case SIGUSR1: 
         n=1; 
         printf("- X - - - -\n");
         break;
      case SIGUSR2: 
         n=2; 
         printf("- - X - - -\n");
         break;
      case SIGALRM:
         n=3;
         printf("- - - X - -\n");
         break;
      case SIGTERM:
         n=4;
         printf("- - - - X -\n");
         break;
      case SIGINT:
         n=5;
         printf("- - - - - X\n");
         break;
   }
   
   int x;
   OZNAKA_CEKANJA[n]++;
   do{
       x = 0;
       for(int j = TEKUCI_PRIORITET+1; j < N; j++){
           if(OZNAKA_CEKANJA[j] != 0)
                x = j;
       }

       if(x>0){
            OZNAKA_CEKANJA[x]--;
            PRIORITET[x] = TEKUCI_PRIORITET;
            TEKUCI_PRIORITET = x;
            dozvoli_prekidanje();
            obrada_prekida(x);
            zabrani_prekidanje();
            TEKUCI_PRIORITET = PRIORITET[x];
       }
   }while(x > 0);

   dozvoli_prekidanje();
}
 
int main ( void )
{
   sigset (SIGUSR1, prekidna_rutina);
   sigset (SIGUSR2, prekidna_rutina);
   sigset (SIGALRM, prekidna_rutina);
   sigset (SIGTERM, prekidna_rutina);
   sigset (SIGINT, prekidna_rutina);
 
   printf("Proces obrade prekida, PID=%d\n", getpid());
   printf("G 1 2 3 4 5\n");
   printf("-----------\n");

   while(1){
        for(int i=0; i<10; i++){
            printf("%d - - - - -\n", i);
            sleep(1);
    }
   }
   printf ("Zavrsio osnovni program\n");
 
   return 0;
}