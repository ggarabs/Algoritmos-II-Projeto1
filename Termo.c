#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>

// VARIAVEIS USADAS CONSTANTEMENTE NO CODIGO
#define MAXN 200
#define TAM 6621
#define WORDLEN 5
#define CHANCES 6
#define DELAY_TIME 200000

// INTERFACE (FUNÇÕES DE INTERFACE APENAS)
void wait_start(){ // Espera o usuário apertar "Enter" para continuar
    getchar();
    system("clear");
}

void delayPrint(){ // Tem objetivo de realizar uma "barra de progresso", simulando um sorteio real pro usuário.
    printf("Sorteando palavra...\n");
       for (int i = 0; i < 10000; i++){ // Laço responsável pelo efeito do programa.
            usleep(DELAY_TIME/1000);
            printf("\rIn progress %d", i/100);
       }
    printf("\n\n\nSorteada!\n\n");
    usleep(10*DELAY_TIME);
    system("clear"); // Limpa a tela para o jogo começar
}
void show_logo(){ // Realiza a exibição linha por linha do logotipo criado no arquivo "logo.txt".
    FILE *aux_arc;
    const char adress[] = "logo.txt";
    aux_arc = fopen(adress, "r"); // Abertura do arquivo

    if(NULL == aux_arc){
        return;
    }

    while(!feof(aux_arc)){ // Enquanto não chegar não imprimir todas as linhas
        char line[200];
        if(NULL != fgets(line, MAXN, aux_arc)){ // Se há conteúdo na linha, então a imprima.
            printf("%s", line);
            usleep(DELAY_TIME);
        }
    }
    fclose(aux_arc); // Fecha o arquivo
    wait_start(); // Espera a confirmação do usuário para continuar
}

int showMenu(bool fim){ // Mostra o menu do jogo.
    // "fim" serve para identificar quando o programa deve exibir o menu novamente.
    char escolha[MAXN]; 
    // armazenamos a escolha numa string pra evitar que o usuario digite numeros com virgula ou ponto e o programa continue
    do{
        // Impressão do menu
        printf("+----------------------------+\n|");
        printf("--------1. -> Start---------|\n|");
        printf("--------2. -> About us------|\n|");
        printf("--------3. -> Exit----------|\n");
        printf("+----------------------------+\n");

        scanf("%s", escolha);

        if(strcmp(escolha, "1") == 0 || strcmp(escolha, "2") == 0 || strcmp(escolha, "3") == 0){ // Se a escolha for 1, 2 ou 3, encerre
            fim = true;
        }else{ // caso contrário, notifique o usuário e repita o menu
            printf("\n\nEscolha incorreta!\nPor favor, escolha uma opção no menu: \n\n");
        }
    }while(!fim);

    int ans = (int)(escolha[0] - '0'); // conversão do valor digitado para inteiro

    return ans; // Retorna a opção escolhida
}

void showInfo(){ // Resultado da escolha 2 (about us) do menu!
    system("clear"); // Limpa a tela
    // Impressão da caixa de informações
    printf("\n+--------Quem somos?--------+------Sobre o Jogo------+------PIX--------+\n|Somos interessados num     | Descubra a palarva     |       cpf:      |\n|mercado entendido como     |Sescreta! É isso!       |  000.000.000-00 |\n|escasso. Criamos com       |Gostou? Avalie Clicando:|                 |\n|intenção de ficarmos       |   [Bom]       [Ruim]   |                 |\n|ricos!!!!                  |kkkkkk caiu na pegadinha|                 |\n+---------------------------+------------------------+-----------------+\n\n");
    usleep(50*DELAY_TIME); // Espera um certo tempo e limpa novamente
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
    bool right_lenght = false; // variável auxiliar para garantir que a palavra sorteada tenha o tamanho correto
    srand(time(NULL)); // semente de acordo com o instante executado

    while (!right_lenght){ // enquanto a palavra sorteada não tiver o tamanho ideal
        int random_number = rand()%TAM + 1; // gera um valor aleatorio. TAM = numero de linhas do arquivo .txt

        archive = fopen(link, "r"); // Abre o arquivo para leitura

        if(NULL == archive){
            printf("Erro ao tentar abrir o arquivo.\n");
            return;
        }
        
        // cada linha é lida até chegar na linha que contém o número sorteado
        for(int i = 1; i <= random_number; i++) fscanf(archive, "%s", word); 

        fclose(archive); // Fecha o arquivo

        if(strlen(word) == WORDLEN){ // Se a palavra tiver o tamanho correto, então encerre o laço
            right_lenght = true;
        }
    }
    strupr(word); // converta a palavra para caixa alta (para comparação e gravação no scores.txt)
    delayPrint(); // produz o efeito de espera para o usuário
}

void data_validation(FILE *archive, char att[], char link[]){ //Valida se a palavra entra nos parâmetros necessários
    bool in_dic = false; // variável que guarda o estado (palavra está ou não no dicionário)

    do{ // Enquanto a palavra digitada não for encontrada no palavras.txt, então repita
        printf("Digite uma palavra de cinco letras: ");
        scanf("%s", att); // Recebe a palavra
        strlwr(att); // Converte para caixa baixa. (Todas as palavras do .txt estão em caixa baixa)

        char line[MAXN]; // Guarda as linhas até chegar na linha procurada
        archive = fopen(link, "r"); // Abre o arquivo no modo leitura

        if(NULL == archive) {
            printf("Erro ao tentar abrir o arquivo.\n");
            return;
        }

        for (int i = 1; i <= TAM; i++){ // Busca linear por todo o arquvio até encontrar a palavra digitada.
            fscanf(archive, "%s", line);
            if(strcmp(line, att) == 0 && strlen(att) == WORDLEN) in_dic = true; // se ela for encontrada e tiver o tamanho ideal, então encerra o laço
        }

        if(!in_dic){ // se não cumprir as condições, então notifica o usuário e repete o processo
            printf("Palavra de tamanho inválido ou não encontrada na base de dados.\n");
            usleep(3*DELAY_TIME);
        }

        printf("\n");

    }while(!in_dic);
    strupr(att); // Após a palavra digitada cumprir as condições, converte-se ela para caixa alta novamente
}

void print_result(char word[], char attemp[], char hist[]){// Realiza o print de uma tabela com a palavra e seu resultado determinado junto da simbologia
// Guarda-se a tabela de histórico numa string, pois todas as tentativas tem um tamanho bem definido
// O histórico contém tentativas e seus resultados.
// Concatena-se a nova tentativa e o histórico.    
    strcat(hist, attemp); 
    printf("+-----------+\n| ");

    for(int j = 0; j < strlen(hist); j++){ // Imprime o histórico antes de gerar o resultado da tentativa atual;
        printf("%c ", hist[j]); // Imprime o caracter
        if(j % 5 == 4) printf("|\n| "); // Se o caracter for o quinto impresso, fechamos a tabela na linha e abrimos na próxima
    }

// ALGORITMO DE RESOLUÇÃO DO PROBLEMA:
/* Inicialmente, todas as letras começam como ausentes nos dois vetores (o da solução e o da marcação do vetor original)
 Primeiro buscamos as letras que estão corretas e no lugar correto e as marcamos com '^' nos dois vetores
 Depois, fazemos uma busca por letras iguais que não estão na posição correta, mas que existem na palavra
 Quando as encontramos, a marcamos na solução e no vetor de marcação
 Como a busca é da esquerda para a direita, então no caso de repetição, apenas a letra mais a esquerda irá ser marcada, conforme exemplo abaixo:

Complexidade de tempo: O(N + N^2) = O(N^2), onde N é o número de caracteres das palavras envolvidas no jogo.
*/

/* EXEMPLO:

PALAVRA SORTEADA: CESTA

TENTATIVA: CERNE

SOLUÇÃO: 
C E R N E
^ ^ x x x

TENTATIVA: CREPE

SOLUÇÃO:
C R E P E
^ x ! x x

*/

    char ans[] = "xxxxx", old[] = "xxxxx"; // ans = solução do problema e old = vetor de marcação das letras

    for(int j = 0; j < WORDLEN; j++){ // Marcação das letras certas na posição certa
        if(attemp[j] == word[j]){
            ans[j] = old[j] = '^';
        }
    }

    for(int j = 0; j < WORDLEN; j++){ // Marcação das letras certas na posição errada
        for(int k = 0; k < WORDLEN; k++){
            if(ans[j] == 'x' && old[k] == 'x' && attemp[j] == word[k]){
                ans[j] = old[k] = '!';
            }
        }
    }

    strcat(hist, ans); // concatenação da solução da palavra no histórico

    for(int j = 0; j < WORDLEN; j++){ // Impressão da solução da tentativa atual
        printf("%c ", ans[j]);
    }

    printf("|\n+-----------+\n\n"); // Fim da tabela
}

int make_attemp(FILE *archive, char link[], char word[], char hist[]){ // Fazer uma tentativa.
// Essa função retorna o número de tentativas para acertar o problema.
// Retorna 0 se o usuário não acertar
    for(int i = 1; i <= CHANCES; i++){ // Repetimos as funções para o número de chances
        printf("%dª Tentativa\n", i);
        char attemp[MAXN];

        data_validation(archive, attemp, link); // obtenção de uma tentativa que tenha o tamanho certo e que esteja no palavras.txt
        usleep(3*DELAY_TIME); // parada no programa

        system("clear"); // limpa-se a tela

        print_result(word, attemp, hist); // impressão do resultado da tentativa
        usleep(3*DELAY_TIME);

        if(strcmp(attemp, word) == 0) return i; // se a tentativa for igual à palavra sorteada, então retorne o número de tentativas 
    }
    return 0; // se chegar aqui, não acertou a palavra
}

void name_validation(char name[]){ // recepção e validação do nick digitado após acertar a palavra
    bool right_lenght = false; // variável auxiliar que ajuda a verificar o tamanho do nick
    do{ // enquanto o tamanho do nick não for ideal, então repita o processo
        printf("Digite seu nick com cinco letras: "); // Recebe o nick
        getchar();
        scanf("%s", name);

        right_lenght = (strlen(name) == WORDLEN); // verifica se o nome tem o tamanho ideal

        if(!right_lenght){ // se não, então notifique o usuário e repita a solicitação
            printf("Nick com tamanho inválido!\n");
            usleep(3*DELAY_TIME);
        }

        printf("\n");

    }while(!right_lenght);
    strupr(name); // Após a obtenção do nome, convertemos tudo para caixa alta pra gravar no scores.txt
}

void finish_game(FILE *output_arc, char link[], char word[], int n_attemps, int time){// Função que finaliza o jogo
    bool win = n_attemps > 0; // se o número de tentativas não for 0, então o usuário acertou
    if(!win){ // se não acertou, notifique-o
        printf("VOCÊ PERDEU! Mais sorte da próxima vez!\n");
        printf("A palavra sorteada foi: %s\n\n", word);
    }else{ // Se sim, solicite o seu nome
        printf("PARABÉNS! Você venceu!\n");

        char player_name[MAXN], std_space[] = "              ";

        name_validation(player_name); // validação do nome pelo número de caracteres e conversão para caixa alta

        output_arc = fopen(link, "a"); // abertura do scores.txt
        if(NULL == output_arc){
            printf("Erro ao tentar abrir o arquivo!");
            return;
        }
        // Gravação das informações
        fprintf(output_arc, "%s%s %s%s %d%s %d\n", player_name, std_space, word, std_space, n_attemps, std_space, time);

        fclose(output_arc); // Fechamento do arquivo
    }
}

void print_scores(){ // Impressão dos vencedores e suas pontuações
    FILE* archive;
    char line[MAXN], link[] = "scores.txt";
    archive = fopen(link, "r"); // Abertura do arquivo

    if(NULL == archive) {
        printf("Erro ao tentar abrir o arquivo.\n");
        return;
    }

    usleep(3*DELAY_TIME); // Delay

    printf("\nEsses sao os scores ate agora:\n");

    usleep(3*DELAY_TIME);

    while(!feof(archive)){ // Enquanto o arquivo não chegar ao fim
        if(NULL != fgets(line, MAXN, archive)){ // Se a linha não for apenas um "\n", então a imprima
            printf("%s", line);
        }
        usleep(DELAY_TIME); // Efeito de delay entre as impressões
    }

    fclose(archive); // Fechamento do arquivo
}

int main(){
    FILE *arc, *output_arc; // variáveis dos arquivos de entrada e saída
    char input_adress[] = "palavras.txt", output_adress[] = "scores.txt"; // endereço desses arquivos
    char drawn_word[MAXN]; // guarda palavra sorteada
    bool on = true; // Auxilia no encerramento do programa pelo menu
    time_t start, end; // Instantes de inicio e fim do programa

    show_logo(); // Mostra-se o logo do jogo

    do{ // Repete o jogo quantas vezes o usuário quiser 
        bool fim = false; // Auxilia na exibição do menu para o usuário digitar uma opção válida
        char a = showMenu(fim); // Exibe e recebe as opções do menu para o usuário 

        switch(a){ // Direciona o programa de acordo com a opção digitada pelo usuário
        case 1: 
            word_sort(arc, input_adress, drawn_word); // Sorteia a palavra

            start = time(NULL); // Inicia o tempo de jogo

            char hist[MAXN] = ""; // Inicializa/Reinicia o histórico da partida
            int got_word = make_attemp(arc, input_adress, drawn_word, hist); // Realiza o jogo e armazena as tentativas até a vitória

            int play_time = time(NULL) - start; // Encerra a contagem e já calcula o intervalo de tempo até o fim

            finish_game(output_arc, output_adress, drawn_word, got_word, play_time); // Finaliza o jogo

            break;
        case 2:
            showInfo(); // Mostra as informações do jogo e dos criadores
            break;
        case 3:
            on = false; // Encerra o funcionamento do programa
            break;
        default:
            printf("Em manutenção!");
            main();
            break;
        }
    }while(on == true); // Enquanto a opção 3 não for digitada

    system("clear"); // Limpa a tela
    printf("Obrigado por jogar! Nós agradecemos <3\n\n"); 
    print_scores(); // Imprime os resultados anteriores
    
    usleep(10*DELAY_TIME);
    
    return 0;
}