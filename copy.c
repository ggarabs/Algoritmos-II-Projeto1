#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>

#define MAXN 200
#define TAM 6621
#define WORDLEN 5
#define CHANCES 6
#define DELAY_TIME 200000
const char MSG[] = "                                            PRESS ENTER KEY TO CONTINUE\n\n\n";

/* O QUE FALTA
- Melhorar interface
- Mais interatividade
- MENU INICIAL (Digite enter para começar/finalizar)
- Header no jogo
*/

void show_logo(){
    FILE *aux_arc;
    const char adress[] = "logo.txt";
    aux_arc = fopen(adress, "r");

    if(NULL == aux_arc){
        return;
    }

    while(!feof(aux_arc)){
        char line[200];
        if(NULL != fgets(line, MAXN, aux_arc)){
            printf("%s", line);
            usleep(DELAY_TIME);
        }
    }
    fclose(aux_arc);
    wait_start();
}

void wait_start(){
    printf(MSG);
    getchar();
    system("clear");
}

void strupr(char string[]){
    for(int i = 0; i < strlen(string); i++) string[i] = toupper(string[i]);
}

void strlwr(char string[]){
    for(int i = 0; i < strlen(string); i++) string[i] = tolower(string[i]);
}

void word_sort(FILE *archive, char link[], char word[]){
    bool right_lenght = false;
    srand(time(NULL));

    while (!right_lenght){
        int random_number = rand()%TAM + 1;

        archive = fopen(link, "r");

        if(NULL == archive){
            printf("Erro ao tentar abrir o arquivo.\n");
            return;
        }

        for(int i = 1; i <= random_number; i++) fscanf(archive, "%s", word);

        fclose(archive);

        if(strlen(word) == WORDLEN){
            right_lenght = true;
        }
    }
    strupr(word);
    printf("%s\n", word);
}

void data_validation(FILE *archive, char att[], char link[]){
    bool in_dic = false;

    do{
        printf("Digite uma palavra de cinco letras: ");
        scanf("%s", att);
        strlwr(att);

        char line[MAXN];

        archive = fopen(link, "r");

        if(NULL == archive) {
            printf("Erro ao tentar abrir o arquivo.\n");
            return;
        }

        for (int i = 1; i <= TAM; i++){
            fscanf(archive, "%s", line);
            if(strcmp(line, att) == 0 && strlen(att) == WORDLEN) in_dic = true;
        }

        if(!in_dic){
            printf("Palavra de tamanho inválido ou não encontrada na base de dados.\n");
            usleep(3*DELAY_TIME);
        }

        printf("\n");

    }while(!in_dic);
    strupr(att);
}

void print_result(char word[], char attemp[], char hist[]){
    printf("+-----------+\n| ");

    for(int j = 0; j < strlen(hist); j++){
        printf("%c ", hist[j]);
        if(j % 5 == 4) printf("|\n| ");
    }

    char ans[] = "xxxxx", old[] = "xxxxx";

    for(int j = 0; j < WORDLEN; j++){
        if(attemp[j] == word[j]){
            ans[j] = old[j] = '^';
        }
    }

    for(int j = 0; j < WORDLEN; j++){
        for(int k = 0; k < WORDLEN; k++){
            if(ans[j] == 'x' && old[k] == 'x' && attemp[j] == word[k]){
                ans[j] = old[k] = '!';
            }
        }
    }

    strcat(hist, ans);

    for(int j = 0; j < WORDLEN; j++){
        printf("%c ", ans[j]);
    }

    printf("|\n+-----------+\n\n");
}

int make_attemp(FILE *archive, char link[], char word[], char hist[]){
    for(int i = 1; i <= CHANCES; i++){
        printf("%dª Tentativa\n", i);
        char attemp[MAXN];

        data_validation(archive, attemp, link);
        usleep(3*DELAY_TIME);

        system("clear");

        strcat(hist, attemp);

        print_result(word, attemp, hist);
        usleep(3*DELAY_TIME);

        if(strcmp(attemp, word) == 0){
            return i;
        }
    }
    return 0;
}

void name_validation(char name[]){
    bool right_lenght = false;
    do{
        printf("Digite seu nick com cinco letras: ");
        getchar();
        scanf("%s", name);

        right_lenght = strlen(name) == WORDLEN;

        if(!right_lenght){
            printf("Nick com tamanho inválido!\n");
            usleep(3*DELAY_TIME);
        }

        printf("\n");

    }while(!right_lenght);
    strupr(name);
}

void finish_game(FILE *output_arc, char link[], char word[], int n_attemps, int time){
    bool win = n_attemps > 0;
    if(!win){
        printf("VOCÊ PERDEU! Mais sorte da próxima vez!\n");
        printf("A palavra sorteada foi: %s\n\n", word);
    }else{
        printf("PARABÉNS! Você venceu!\n");

        char player_name[MAXN], std_space[] = "              ";

        name_validation(player_name);

        output_arc = fopen(link, "a");
        if(NULL == output_arc){
            printf("Erro ao tentar abrir o arquivo!");
            return;
        }

        fprintf(output_arc, "%s%s %s%s %d%s %d%s\n", player_name, std_space, word, std_space, n_attemps, std_space, time, std_space);

        fclose(output_arc);
    }
}

int main(){
    FILE *arc, *output_arc;
    char input_adress[] = "palavras.txt", output_adress[] = "scores.txt";
    char drawn_word[MAXN], hist[MAXN] = "";
    time_t start, end;

    show_logo();

    word_sort(arc, input_adress, drawn_word);

    start = time(NULL);

    int got_word = make_attemp(arc, input_adress, drawn_word, hist);

    int play_time = time(NULL) - start;

    finish_game(output_arc, output_adress, drawn_word, got_word, play_time);

    return 0;
}