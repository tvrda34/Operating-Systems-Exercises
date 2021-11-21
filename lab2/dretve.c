#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <malloc.h>

int brojDretvi, brojIteracija;
int a;

void *dretve(void *rbr){
    for(int i=0; i<brojIteracija; i++){
        a=a+1;
    }
    return NULL;
}

int main(int argc, char *argv[]){
    pthread_t *t;
    a=0;

    if(argc != 3){
        printf("Potrebno je unijeti dva parameta 1. broj dretvi, 2. broj iteracija\n");
        exit(1);
    }
    
    brojDretvi = atoi(argv[1]);
    brojIteracija = atoi(argv[2]);

    t = malloc(brojDretvi * sizeof(pthread_t));

    for(int i = 0; i<brojDretvi; i++){
        if(pthread_create(&t[i], NULL, dretve, NULL)){
            fprintf (stderr, "Error\n");
            exit(1);
        }
    }

    for(int j=0; j<brojDretvi; j++){
        pthread_join( t[j], NULL);
    }

    printf("A=%d\n", a);
    return 0;
}