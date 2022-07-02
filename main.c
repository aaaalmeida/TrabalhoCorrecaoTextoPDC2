#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>
#include <time.h> // time é opcional, so inclui para poder ver o tempo de execução do programa

// #include "cabecalho.h"

// depois joga essas funcoes e constante td no header
#define MIN3(a, b, c) ((a) < (b) ? ((a) < (c) ? (a) : (c)) : ((b) < (c) ? (b) : (c)))
#define TAM_MAX_PALAVRA 50
#define mostraNoTerminal 0


/*typedef struct no{
    // nao vou mais usar lista encadeada :(
    unsigned char palavraDicionario[TAM_MAX_PALAVRA];
    struct no *proximoNo;  // aponta para a proxima palavra da lista
    struct no *proximaLetra; // aponta para a primeira palavra que começa com a proxima letra
} dicionario;
*/

/*
void leituraDicionarioLISTA(FILE *dic, dicionario *comeco){
// PILHA É UMA MERDA E VAI FAZER O PROGRAMA FICAR LENTO, NÃO VALE A PENA CONTINUAR COM A IDEIA
// (mas eu vou tentar msm assim)

        Basicamente esta função cria uma fila encadeada
        lendo as palavras do arquivo dicionario.txt.
        O ponteiro proximoNo da struct dicionario aponta para a proxima palavra
        e o ponteiro proximaLetra aponta para a primera palavra que começa com uma nova letra.


// ESSA FUNCAO CONSEGUE LER E IMPRIMIR, O PROBLEMA ATE AGORA É QUE ASSIMILAR primeiraNovaLetra
// À PRIMEIRA PALAVRA COM LETRA NOVA.

    char puxaDicionario[TAM_MAX_PALAVRA];
    dicionario *aux1, *aux2, *primeiraNovaLetra;
    aux1 = malloc(sizeof(dicionario));
    primeiraNovaLetra = malloc(sizeof(dicionario));

    fgets(comeco->palavraDicionario, TAM_MAX_PALAVRA, dic);
    comeco->proximoNo = aux1;
    primeiraNovaLetra = comeco; // PROVAVELMENTE O PROBLEMA TA AQ
    // ESSE PONTEIRO TALVEZ SEJA UM PONTEIRO PRA PONTEIRO????

    // minha ideia é cria palavras chaves que sao as primeiras palavras a comecarem com outra letra
    // o problema é se o ponteiro auxiliar primeiraNovaLetra é uma copia do original ou o apontamento
    // if(primeiraNovaLetra->palavraDicionario[0] != aux1->palavraDicionario[0]) comeco->proximaLetra = aux1;

    int i=0;
    while(fgets(puxaDicionario, TAM_MAX_PALAVRA, dic)){
        // printf(puxaDicionario);

        if(i=0){
            i=1;
            aux1 = malloc(sizeof(dicionario));
            strcpy(aux1->palavraDicionario, puxaDicionario);
            aux1->proximoNo = aux2;
            aux1->proximaLetra = NULL;
        // printf(aux1->palavraDicionario);

// PROVAVELMENTE VOU TER Q MUDAR ESSE IF
            if(primeiraNovaLetra->palavraDicionario[0] != aux1->palavraDicionario[0]){
            primeiraNovaLetra->proximaLetra = aux1;
            primeiraNovaLetra = aux1;
            }
        }

        else{
            i=0;
            aux2 = malloc(sizeof(dicionario));
            strcpy(aux2->palavraDicionario, puxaDicionario);
            aux2->proximoNo = aux1;
            aux2->proximaLetra = NULL;
        // printf(aux2->palavraDicionario);

// ESSE AQ MUDA JUNTO
            if(primeiraNovaLetra->palavraDicionario[0] != aux2->palavraDicionario[0]){
            primeiraNovaLetra->proximaLetra = aux2;
            primeiraNovaLetra = aux2;
            }
        }
    }
}
*/

int hamming(int tamanho, unsigned char *s1, unsigned char *s2) {
    /*  A distância de Hamming entre duas strings de MESMO COMPRIMENTO é
        o número de posições nas quais elas diferem entre si.
        Vista de outra forma, ela corresponde ao menor número de substituições necessárias
        para transformar uma string na outra, ou o número de erros que transformaram uma na outra.
    */
    // https://pt.wikipedia.org/wiki/Distância_de_Hamming#Exemplos

    // NAO VOU MAIS USAR ESSA MERDA
    int distancia = 0;
    for (int i = 0; i < tamanho; i++) {
        if (*s1 != *s2)
            distancia++;
        s1++;
        s2++;
    }
    return distancia;
}

int levenshtein(unsigned char *s1, unsigned char *s2) {
    /*  A distância Levenshtein ou distância de edição entre dois "strings"
        (duas sequências de caracteres) é dada pelo número mínimo de operações
        necessárias para transformar um string no outro.
        Entendemos por "operações" a inserção, deleção ou substituição de um carácter
    */
    // https://pt.wikipedia.org/wiki/Distância_Levenshtein#Limites_superior_e_inferior

    int s1len, s2len, x, y, lastdiag, olddiag;
    s1len = strlen(s1);
    s2len = strlen(s2);
    int column[s1len + 1];
    for (y = 1; y <= s1len; y++)
        column[y] = y;
    for (x = 1; x <= s2len; x++) {
        column[0] = x;
        for (y = 1, lastdiag = x - 1; y <= s1len; y++) {
            olddiag = column[y];
            column[y] = MIN3(column[y] + 1, column[y - 1] + 1, lastdiag + (s1[y - 1] == s2[x - 1] ? 0 : 1));
            lastdiag = olddiag;
        }
    }

    // if(mostraNoTerminal)    printf("distancia: %d \n", column[s1len] );
    return column[s1len];
}

unsigned int achaTamanhoArquivo(FILE *arq) {
    // funcao para achar a quantidade de linhas do arquivo dicionario
    
    fseek(arq, 0, SEEK_SET);

    char c = fgetc(arq);
    unsigned int tamanho = 1;
    while (c != EOF) {
        if (c == '\n')
            tamanho++;
        c = fgetc(arq);
    }
    if(mostraNoTerminal)    printf("qtd de linhas do arquivo: %d\n", tamanho);
    return tamanho;
}

void testeArquivo(FILE *arquivo) {
    // funcao para testar abertura do arquivo
    if (!arquivo) {
        printf("ERRO AO ABRIR ARQUIVO");
        exit(1);
    }
}

int testaLetra(unsigned char c) {
    /*  funcao retorna positivo se o caracterer é válido e
        negativo se o caractere não for válido.
        A definição de caracteres válidos nesse código são todas as letras minúsculas
        com ou sem acento e cedilha.
    */
    // https://gdhpress.com.br/wp-content/uploads/2019/07/tabelaASCII.pdf

    if ((c >= 65 && c <= 90) ||     // alfabeto maiusculo
        (c >= 97 && c <= 122) ||    // alfabeto minusculo
        (c >= 130 && c <= 141) ||
        (c >= 147 && c <= 151) ||
        (c >= 160 && c <= 163) ||
        (c == 198) ||
        (c == 228))
        return 1;
    else
        return 0;
}

void leituraDicionarioVETOR(FILE *dic, unsigned char bancoPalavras[][TAM_MAX_PALAVRA], unsigned int tamanhoArquivo) {
    // preenchimento do vetor com as palavras do dicionario
   
    fseek(dic, 0, SEEK_SET);
    for (int i=0; i<tamanhoArquivo; i++) {
        fgets(bancoPalavras[i], TAM_MAX_PALAVRA, dic);

        int tamPalavra = strlen(bancoPalavras[i]);
        for (int j=0; j<tamPalavra; j++) {
            if (!testaLetra(bancoPalavras[i][j])) {
                bancoPalavras[i][j] = '\0';
                break;
            }
        }
        if(mostraNoTerminal)    printf("%s \n", bancoPalavras[i]);
    }
}

void limpaPonteiroPalavra(unsigned char *palavra, unsigned char *comeco) {
    // funcao para limpar e resetar o ponteiro que armazena a palavra testada
    palavra = comeco;
    for (int i = 0; i < TAM_MAX_PALAVRA; i++, palavra++)
        *palavra = '\0';
    palavra = comeco;
}

int buscaBinaria(unsigned char lista[][TAM_MAX_PALAVRA], unsigned char *busca, unsigned int tamanhoLista, unsigned int tamanhoPalavra) {
    // Busca uma palavra no dicionario atraves de busca binária.
    // Retorna positivo se achar ou negativo se não achar

    int inf = 0;
    int sup = tamanhoLista - 1;
    int meio;
    while (inf <= sup) {
        meio = (inf + sup) / 2;
        if (!strncmp(busca, lista[meio], tamanhoPalavra))         
            return 1;                                             // encontrado
        else if (strncmp(busca, lista[meio], tamanhoPalavra) < 0) 
            sup = meio - 1;
        else 
            inf = meio + 1;
    }
    return 0; // não encontrado
}

int sugestaoPalavra(unsigned char lista[][TAM_MAX_PALAVRA], unsigned char *busca, unsigned int tamanhoLista, unsigned int tamanhoPalavra, FILE *arqCopia) {
    // Mesma coisa que a busca binaria so que com levenshtein no meio 
    // para achar as palavras parecidas

    int achouSugestao=0;
    int inf = 0;
    int sup = tamanhoLista - 1;
    int meio;
    while (inf <= sup) {
        meio = (inf + sup) / 2;

        if (levenshtein(busca, lista[meio]) <= 2) {
            if(mostraNoTerminal)    printf("(%s)", lista[meio]);
            fprintf(arqCopia, "(%s)", lista[meio]);
            achouSugestao = 1;
        }
        if (strncmp(busca, lista[meio], tamanhoPalavra) < 0) { // busca é menor
            sup = meio - 1;
        }
        else // busca é maior
            inf = meio + 1;
    }

    // essa função tem um retorno mas eu esqueci oque ia fazer com ele, então deixa assim mesmo
    if(achouSugestao) return 1;
    else return 0;
}

int main() {
    setlocale(LC_ALL, "");

    clock_t start = clock();

    FILE *arq = fopen("textosExemplo\\palavra.txt", "r");  // texto exemplo
    testeArquivo(arq);
    FILE *dic = fopen("textosExemplo\\dicionario.txt", "r"); // banco de palavras
    testeArquivo(dic);
    FILE *arqCopia = fopen("textosExemplo\\resposta.txt", "w+");  // texto formatado
    testeArquivo(arqCopia);

    //COMECO DO DICIONARIO NA LISTA ENCADEADA (ideia ruim)
    // dicionario *comecoLista = malloc(sizeof(dicionario));   
    // leituraDicionarioLISTA(arq, comecoLista);

    unsigned int qtdPalavrasDicionario = achaTamanhoArquivo(dic);
    printf("teste 1\n");
    unsigned char bancoPalavras[qtdPalavrasDicionario][TAM_MAX_PALAVRA];
    printf("teste 2\n");

    leituraDicionarioVETOR(dic, bancoPalavras, qtdPalavrasDicionario); // escrita do dicionario na RAM
    printf("teste 3\n");

    unsigned char *palavra = malloc(sizeof(char) * TAM_MAX_PALAVRA); // ponteiro para teste de comparação com o dicionario
    unsigned char *resetaPalavra = palavra;                          // ponteiro para voltar para posição original

    unsigned char letra;
    short int fimDePalavra; // poderia ser booleana
    short int achouDicionario;  // poderia ser booleana
    printf("teste 4\n");

    if(mostraNoTerminal)    printf("\n--------------------------------\n");
    do{
        // leitura e teste de caractere
        letra = fgetc(arq);
        fimDePalavra = testaLetra(letra);

        if(letra == 255) break;
        
        // adiciona o caractere e sobe o ponteiro na string contando que seja valido
        if(fimDePalavra) {
            *palavra = letra;
            palavra++;
            *palavra = '\0';
        }

        /*  Quando o programa detecta um caractere não valido ele reconhece
            o conjunto de caracteres anteriores como uma palavra.
            Então o ponteiro volta para a primeira posição e usa
            as funções hamming e levenshtein
        */
    
        else{
            palavra = resetaPalavra;    // volta string para posicao original para comparar com o dicionario
            achouDicionario = buscaBinaria(bancoPalavras, palavra, qtdPalavrasDicionario, strlen(palavra));

            if(achouDicionario) {    // encontrou no dicionario
                if(mostraNoTerminal){
                    printf("%s", palavra);
                    printf("%c", letra);
                }
                fprintf(arqCopia, "%s", palavra);
                fprintf(arqCopia, "%c", letra);
                limpaPonteiroPalavra(palavra, resetaPalavra);
            }
    
            else {    // não encontrou a palavra no dicionario
                if(mostraNoTerminal)    printf("[%s]", palavra);
                fprintf(arqCopia, "[%s]", palavra);
                sugestaoPalavra(bancoPalavras, palavra, qtdPalavrasDicionario, strlen(palavra), arqCopia);
                
                if(mostraNoTerminal)    printf("%c", letra);
                fprintf(arqCopia, "%c", letra);
                limpaPonteiroPalavra(palavra, resetaPalavra);
            }
        palavra = resetaPalavra;    // volta string para posicao original para armazenar uma nova palavra
        }
    } while(letra!=255);
    
    if(mostraNoTerminal)    printf("\n--------------------------------\n");


    printf("teste 5\n");
    fseek(arq, 0, SEEK_SET);
    fseek(dic, 0, SEEK_SET);
    fseek(arqCopia, 0, SEEK_SET);

    printf("teste 6\n");
    fclose(arq);
    fclose(dic);
    fclose(arqCopia);

    printf("teste 7\n");
    clock_t end = clock();
    double tempo_duracao_programa = (double)(end - start) / CLOCKS_PER_SEC;
    printf("\nTEMPO DE DURACAO DO PROGRAMA: %.10lfsec", tempo_duracao_programa);
    printf("\nFIM DO PROGRAMA");

    exit(0);
}
