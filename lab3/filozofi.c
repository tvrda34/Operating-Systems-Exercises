#include <pthread.h>
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <unistd.h>

#define BR_FILOZOFA 5
pthread_mutex_t m;     //monitor
pthread_cond_t red[BR_FILOZOFA]; //redovi uvjeta

char filozof[] = {'0', '0', '0', '0', '0'};
int vilica[BR_FILOZOFA] ={1, 1, 1, 1, 1};

void misliti(){
    sleep(3);
}

void ispisi_stanje(int n){
    for(int i = 0; i< (sizeof(filozof) / sizeof(filozof[0])); i++){
        printf("%c ", filozof[i]);
    }
    printf("(%d)\n", n+1);
}

void jesti(int n) {
    pthread_mutex_lock (&m);
    filozof[n] = 'o';
    while(vilica[n] == 0 || vilica[(n + 1) % 5] == 0)
        pthread_cond_wait (&red[n], &m);
    vilica[n] = vilica[(n + 1) % 5] = 0;
    filozof[n] = 'X';
    ispisi_stanje(n);
    pthread_mutex_unlock (&m);

    sleep(2); //jede

    pthread_mutex_lock (&m);
    filozof[n] = 'O';
    vilica[n] = vilica[(n + 1) % 5] = 1;
    pthread_cond_signal (&red[(n - 1) % 5]);
    pthread_cond_signal (&red[(n + 1) % 5]);

    ispisi_stanje(n);
    pthread_mutex_unlock (&m);

}


void *filozofi (void *p) {
	int n = *((int *) p);

    while(1){
        misliti();
        jesti(n);
    }

    return NULL;
}

int main () {
	pthread_t t[BR_FILOZOFA];
	int i;
    int Br[BR_FILOZOFA];
    Br[0] = 0;
    Br[1] = 1;
    Br[2] = 2;
    Br[3] = 3;
    Br[4] = 4;

	pthread_mutex_init (&m, NULL);
	pthread_cond_init (&red[0], NULL);
	pthread_cond_init (&red[1], NULL);
    pthread_cond_init (&red[2], NULL);
	pthread_cond_init (&red[3], NULL);
    pthread_cond_init (&red[4], NULL);


	for (i = 0; i < BR_FILOZOFA; i++) {
		pthread_create (&t[i], NULL, filozofi, &Br[i]);
    }

	for (i = 0; i < BR_FILOZOFA; i++)
		pthread_join (t[i], NULL);

	pthread_mutex_destroy (&m);
	pthread_cond_destroy (&red[0]);
	pthread_cond_destroy (&red[1]);
    pthread_cond_destroy (&red[2]);
	pthread_cond_destroy (&red[3]);
    pthread_cond_destroy (&red[4]);

	return 0;
}