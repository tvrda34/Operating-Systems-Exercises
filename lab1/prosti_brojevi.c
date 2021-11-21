#define _XOPEN_SOURCE
#define _XOPEN_SOURCE_EXTENDED

#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<unistd.h>
#include<sys/time.h>
#include<math.h>

int pauza = 0;
unsigned long broj = 1000000001;
unsigned long zadnji = 1000000001;

void periodicki_ispis(int sig){
    printf("zadnji prost broj = %lu\n", zadnji);
}

void postavi_pauzu(int sig){
    pauza = 1 - pauza;
	if(pauza == 1){
		printf("zaustavljanje...\n");
	}
	else{
		printf("pokretanje...\n");
	}
}

void prekini(int sig){
    printf("zadnji prost broj = %lu\n", zadnji);
	printf("QUIT\n");
    exit(0);
}

int prost ( unsigned long n ) { //funkcija za provjeru kao u uputi labosa
	unsigned long i, max;

	if ( ( n & 1 ) == 0 ) /* je li paran? */
		return 0;

	max = sqrt ( n );
	for ( i = 3; i <= max; i += 2 )
		if ( ( n % i ) == 0 )
			return 0;

	return 1; /* broj je prost! */
}

int main(){
    struct itimerval t;


	sigset ( SIGALRM, periodicki_ispis );
    sigset ( SIGTERM, prekini);
	sigset ( SIGINT, postavi_pauzu);

	t.it_value.tv_sec = 5;
	t.it_value.tv_usec = 0;
	
	t.it_interval.tv_sec = 5;
	t.it_interval.tv_usec = 0;

	setitimer ( ITIMER_REAL, &t, NULL );
    while(1){
        if(prost(broj) == 1){
            zadnji = broj;
		}
    broj++;
    while(pauza == 1){
        pause();
	}
    }

    return 0;
}