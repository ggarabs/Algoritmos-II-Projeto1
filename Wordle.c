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

/* O QUE FALTA
- Melhorar interface
- Gerar função que coloca tudo caixa alta
- Jogabilidade (tempo de pausa)
- Mais interatividade
*/

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
            for(int i = 0; i < strlen(word); i++) word[i] = toupper(word[i]);
        }
    }
}

void data_validation(FILE *archive, char att[], char link[]){
    bool in_dic = false;

    do{
        printf("Digite uma palavra de cinco letras: ");
        scanf("%s", att);

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

        if(!in_dic) printf("Palavra de tamanho inválido ou não encontrada na base de dados.\n");

        printf("\n");

    }while(!in_dic);
    for(int i = 0; i < strlen(att); i++) att[i] = toupper(att[i]);
}

void print_result(char word[], char attemp[]){
    printf("+-----------+\n| ");

    for(int j = 0; j < strlen(attemp); j++){
        printf("%c ", attemp[j]);
    }

    printf("|\n| ");

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

    for(int j = 0; j < WORDLEN; j++){
        printf("%c ", ans[j]);
    }

    printf("|\n+-----------+\n\n");
}

int make_attemp(FILE *archive, char link[], char word[]){
    for(int i = 1; i <= CHANCES; i++){
        printf("%dª Tentativa\n", i);
        char attemp[MAXN];

        data_validation(archive, attemp, link);

        print_result(word, attemp);

        if(strcmp(attemp, word) == 0){
            return i;
        }
    }
    return 0;
}

void finish_game(FILE *output_arc, char link[], char word[], int n_attemps, int time){
    bool win = n_attemps > 0;
    if(!win){
        printf("VOCÊ PERDEU! Mais sorte da próxima vez!\n");
        printf("A palavra sorteada foi: %s\n\n", word);
    }else{
        printf("PARABÉNS! Você venceu!\n");

        char player_name[MAXN], std_space[] = "              ";

        printf("Digite seu nome: ");
        getchar();
        fgets(player_name, MAXN, stdin);
        player_name[strlen(player_name)-1] = '\0';
        for(int i = 0; i < strlen(player_name); i++) player_name[i] = toupper(player_name[i]);

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
    char drawn_word[MAXN];
    time_t start, end;

    word_sort(arc, input_adress, drawn_word);

    start = time(NULL);

    int got_word = make_attemp(arc, input_adress, drawn_word);

    int play_time = time(NULL) - start;

    finish_game(output_arc, output_adress, drawn_word, got_word, play_time);

    return 0;
}