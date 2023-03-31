#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

#define MAXN 200
#define TAM 6621
#define WORDLEN 5

int main(){
    FILE *arc;
    const char adress[] = "logo.txt";
    arc = fopen(adress, "r");


    if(NULL == arc){
        return -1;
    }

    while(!feof(arc)){
        char line[200];
        if(NULL != fgets(line, MAXN, arc)){
            printf("%s", line);
            usleep(250000);
        }
    }

    fclose(arc);

    return 0;

}