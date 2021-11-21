#include<stdio.h>
#include<stdlib.h>
#include<time.h>

int storage_size;
int polje[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0}; //hardcodirano polje za ispis spremnika
int polje_size = sizeof(polje) / sizeof(polje[0]); //velicina polja za ispis
char *storage; //storage

void storage_insert(int counter, char elem, int start){
    while (counter > 0){
        storage[start] = elem;
        start++; 
        --counter;
    }
}


int storage_add(char elem, int counter){
    int position = 0;
    int min = storage_size;
    int pun = 1, change = 1;

    for(int i=0; i < storage_size; i++){
        if(storage[i] == '-'){
            int chck = 0;
            int pos = i;
            pun = 0;

            while(storage[i] == '-'){
                ++chck;
                ++i;
            }
           
            if(chck <= min && chck >= counter){
                min = chck;
                position = pos;
                change = 0;
            }
        }
    }
  
    if(min < counter || change || pun){
        printf("Nema dovoljno mjesta -> probaj pozvati Garbage collector (G)\n");
        return 0;
    }
    else{
        storage_insert(counter, elem, position);
        return 1;
    }
}

void storage_remove(char elem){
    for(int i = 0; i < storage_size; i++){
        if(storage[i] == elem){
            storage[i] = '-';
        } 
    }
}

void garbage_collector(){
    int counter = 0;
    char *new_storage;
    new_storage = (char*)malloc(storage_size * sizeof(char));
    
    for(int i = 0; i < storage_size; i++){
        if(storage[i] != '-'){
            new_storage[counter] = storage[i];
            ++counter;
        }
    }
    for(int i = counter; i < storage_size; i++){
        new_storage[i] = '-';
    }

    for(int i=0; i < storage_size; i++){
        storage[i] = new_storage[i];
    }
}

void storage_print(){
    //ispis positions
    for(int i=0; i < storage_size; i++){
        printf("%d", polje[i % polje_size]);
    }
    printf("\n");

    //print storage
    for(int i=0; i < storage_size; i++){
        printf("%c", storage[i]);
    }
    printf("\n");
}

int main (int argc, char *argv[]){
    
    if(argc != 2){
        printf("Potrebno je upisati jedan argument -> veličinu spremnika\n");
        exit(0);
    }

    storage_size=atoi(argv[1]);

    storage = (char*)malloc(storage_size * sizeof(char));
    if (storage == NULL) {
        printf("Memorija se nije mogla alocirati\n");
        exit(0);
    }

    //punjenje praznim mjestima
    for(int i = 0; i < storage_size; i++){
        storage[i] = '-';
    }

    //ispis spremnika inicijalan -> prazan 
    storage_print();

    char comand;
    char counter = 97;
    srand ((unsigned int) time(NULL));
    int random;

    while(1){
        scanf(" %c", &comand);
        //dodavanje u sremnik
        if(comand == 90){
            random = rand() % 10;
            
            //da se ne generira prazan zahtijev
            if (random == 0){
                random = 1;
            }
            
            printf("Novi zahtjev %c za %d spremnickih mjesta\n", counter, random);

            //dodavanje i ispis spremnika
            if(storage_add(counter, random)){
                storage_print();
                counter++;
            }
        }

        //brisanje iz spremnika
        if(comand == 79){
            char zahtijev;
            printf("Koji zahtjev treba osloboditi?\n");
            scanf(" %c", &zahtijev);
            printf("Oslobada se zahtjev %c\n", zahtijev);

            //micanje i ispis
            storage_remove(zahtijev);
            storage_print();
        }

        //garbage collector
        if(comand == 71){
            printf("Garbage collector radi\n");
            garbage_collector();
            storage_print();
        }

        if(comand != 90 && comand != 71 && comand != 79){
            printf("Pogresna tipka je stisnuta (case sensitive!), upute: \n");
            printf("Z - dodavanje\n");
            printf("O - otpustanje\n");
            printf("G - garabge collector (preslagivanje spremnika)\n");
        }
       
    }
    return 0;
}