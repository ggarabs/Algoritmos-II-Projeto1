#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>

#define MAXN 200
#define TAM 6621
#define WORDLEN 5
#define CHANCES 6
#define DELAY_TIME 200000

/* O QUE FALTA
- MENU INICIAL (Digite enter para começar/finalizar)(x)
- Header no jogo()
- "A PALAVRA ESTA SENDO GERADA! ESPERE UNS INSTANTES
PALAVRA SORTEADA! BOM JOGO!"(x) // Algo assim
- MENU FINAL + OPÇ~AO DE VER SCORES()
- LIMPAR ENTRADAS (ESPAÇOS A FRENTE E A TRAS)()
*/

// INTERFACE ( FUNÇÕES DE INTERFACE APENAS)
void wait_start(){ // Espera o usuário teclar para continuar
    getchar();
    system("clear");
}
void delayPrint() // Tem objetivo de realizar uma "barra de progresso", dando o sentimento pro usuário de progresso
{
    printf("Sorteando palavra...\n");
    int i = 0;
       for (i = 0; i < 10000; i++){
            usleep(200);
            printf("\rIn progress %d", i/100);
       }
       printf("\n");
    printf("\n\nSorteada!\n\n");
    usleep(900000);
    system("clear");
}
void show_logo(){ // Realiza a Exibição do Logo de modo pausado
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
int showMenu(int fim) // Mostra o menu do jogo!
{
    if(fim == 0){
        printf("+----------------------------+\n|");
        printf("--------1. -> Start---------|\n|");
        printf("--------2. -> About us------|\n|");
        printf("--------3. -> Exit----------|\n");
        printf("+----------------------------+\n");
    }
    int escolha = 0;
    scanf("%d", &escolha);
    if(escolha != 1 && escolha != 2 && escolha != 3)
    {
        printf("\n\nEscolha incorreta!\n\n");
        showMenu(fim);
    }else
    {
        return(escolha);
    }
}
void showInfo() //Resultado da escolha 2 (about us) do menu!
{
    system("clear");
    printf("\n+--------Quem somos?--------+------Sobre o Jogo------+------PIX--------+\n|Somos interessados num     | Descubra a palarva     |       cpf:      |\n|mercado entendido como     |Sescreta! É isso!       |  000.000.000-00 |\n|escasso. Criamos com       |Gostou? Avalie Clicando:|                 |\n|intenção de ficarmos       |   [Bom]       [Ruim]   |                 |\n|ricos!!!!                  |kkkkkk caiu na pegadinha|                 |\n+---------------------------+------------------------+-----------------+\n\n");
    usleep(9900000);
    system("clear");
}
// SERVICE (FUNÇÕES DE PROCESSAMENTO)


void strupr(char string[]){ // Deixa as letras de uma string em caixa alta! 
    for(int i = 0; i < strlen(string); i++) string[i] = toupper(string[i]);
}

void strlwr(char string[]){// Deixa as letras de uma string em caixa baixa!
    for(int i = 0; i < strlen(string); i++) string[i] = tolower(string[i]);
}

void word_sort(FILE *archive, char link[], char word[]){ // Função que realiza a escolha randomica da palavra
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
//    printf("%s\n", word);
    delayPrint();
}

void data_validation(FILE *archive, char att[], char link[]){//Valida se a palavra entra nos parâmetros necessários
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

void print_result(char word[], char attemp[], char hist[]){// Realiza o print de uma tabela com a palavra e seu resultado determinado junto da simbologia
    strcat(hist, attemp);
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

int make_attemp(FILE *archive, char link[], char word[], char hist[]){// Junção das funções ao vivo
    for(int i = 1; i <= CHANCES; i++){
        printf("%dª Tentativa\n", i);
        char attemp[MAXN];

        data_validation(archive, attemp, link);
        usleep(3*DELAY_TIME);

        system("clear");

        print_result(word, attemp, hist);
        usleep(3*DELAY_TIME);

        if(strcmp(attemp, word) == 0) return i;
    }
    return 0;
}

void name_validation(char name[]){//
    bool right_lenght = false;
    do{
        printf("Digite seu nick com cinco letras: ");
        getchar();
        scanf("%s", name);

        right_lenght = (strlen(name) == WORDLEN);

        if(!right_lenght){
            printf("Nick com tamanho inválido!\n");
            usleep(3*DELAY_TIME);
        }

        printf("\n");

    }while(!right_lenght);
    strupr(name);
}

void finish_game(FILE *output_arc, char link[], char word[], int n_attemps, int time){// Função que realiza o finalizamento do jogo
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

        fprintf(output_arc, "%s%s %s%s %d%s %d\n", player_name, std_space, word, std_space, n_attemps, std_space, time);

        fclose(output_arc);
    }
}

void print_scores(){
        FILE* archive;
        char line[MAXN], link[] = "scores.txt";
        archive = fopen(link, "r");

        if(NULL == archive) {
            printf("Erro ao tentar abrir o arquivo.\n");
            return;
        }

        usleep(3*DELAY_TIME);

        printf("\nEsses sao os scores ate agora:\n");
    
        usleep(3*DELAY_TIME);

        while(!feof(archive)){
            fgets(line, MAXN, archive);
            printf("%s", line);
        }

        fclose(archive);

}

int main(){
    bool on = true;
    FILE *arc, *output_arc;
    char input_adress[] = "palavras.txt", output_adress[] = "scores.txt";
    char drawn_word[MAXN];
    time_t start, end;

    show_logo();

    do{
        int a = showMenu(0);

        switch(a){
        case(1):
            word_sort(arc, input_adress, drawn_word);

            start = time(NULL);

            char hist[MAXN] = "";
            int got_word = make_attemp(arc, input_adress, drawn_word, hist);

            int play_time = time(NULL) - start;

            finish_game(output_arc, output_adress, drawn_word, got_word, play_time);

            break;
        case 2:
            showInfo();
            break;
        case 3:
            on = false;
            break;
        default:
            printf("Em manutenção!");
            main();
            break;
        }
    }while (on == true);

    system("clear");
    printf("Obrigado por jogar! Nós agradecemos <3\n\n");
    print_scores();
    
    usleep(9000000);
    
    return 0;
}