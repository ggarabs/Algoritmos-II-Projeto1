#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <stdlib.h>

#define MAXN 200
#define TAM 6621
#define WORDLEN 5

int main(){
    FILE *arc;
    const char input_adress[] = "palavras.txt";
    char drawn_word[] = "pasta";

    const int chances = 6;
    bool got_word = false;
    int n_attemps = 0;
    time_t start, end;

    start = time(NULL);

    for(int i = 1; i <= chances && !got_word; i++){

        printf("%dª Tentativa\n", i);

        char attemp[MAXN];
        n_attemps++;

        bool in_dic = false;

        printf("Digite uma palavra de cinco letras: ");
        scanf("%s", attemp);

        if(strcmp(attemp, drawn_word) == 0){
            got_word = true;
        }

        printf("+-----------+\n| ");

        for(int j = 0; j < strlen(attemp); j++){
            printf("%c ", toupper(attemp[j]));
        }

        printf("|\n| ");

        char ans[] = "xxxxx";
        char old[] = "xxxxx";

        for(int j = 0; j < WORDLEN; j++){
            if(attemp[j] == drawn_word[j]){
                ans[j] = '^';
                old[j] = '^';
            }
        }

        for(int j = 0; j < WORDLEN; j++){
            for(int k = 0; k < WORDLEN; k++){
                if(ans[j] == 'x' && old[k] == 'x' && attemp[j] == drawn_word[k]){
                    ans[j] = '!';
                    old[k] = '!';
                }
            }
        }

        for(int j = 0; j < WORDLEN; j++){
            printf("%c ", ans[j]);
        }

        printf("|\n+-----------+\n");

        printf("\n");
    }

    int play_time = time(NULL) - start;

    FILE *output_arc;
    const char output_adress[] = "scores.txt";

    if(!got_word){
        printf("VOCÊ PERDEU! Mais sorte da próxima vez!\n");
        printf("A palavra sorteada foi: %s\n\n", drawn_word);
    }else{
        printf("PARABÉNS! Você venceu!\n");

        char player_name[MAXN];
        char std_space[] = "              ";

        printf("Digite seu nome: ");
        getchar();
        fgets(player_name, MAXN, stdin);

        int tam = strlen(player_name);
        player_name[tam-1] = '\0';

        output_arc = fopen(output_adress, "a");

        if(NULL == output_arc){
            printf("Erro ao tentar abrir o arquivo!");
            return -1;
        }

        fprintf(output_arc, "%s%s %s%s %d%s %d%s\n", player_name, std_space, drawn_word, std_space, n_attemps, std_space, play_time, std_space);

        fclose(output_arc);
        
    }

    return 0;

}