#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>
#include <time.h>   // time é opcional, so inclui para poder ver o tempo de execução do programa

// #include "cabecalho.h"


/*  CARACTERES IMPORTANTES DA TABELA ASCII:

    
    Caracteres de controle ASCII: 0 até 31

    Alfabeto Maiusculo: 65 até 90
    Alfabeto Minusculo: 97 até 122
    Algarismos : 48 até 57
    Cedilha: 199 e 231
    
    Espaço: 32
    Ponto final: 46
    Exclamação: 33
    Interrogação: 63
    Vírgula: 44
    Parenteses: 40 e 41
    Chaves: 123 e 124
    Colchetes: 91 e 93
    Dois pontos: 58
    Ponto vírgula: 59

    ACENTOS:
    Letra A Maiuscula:  193 até 196
    Letra E Maiuscula:  200 até 203
    Letra I Maiuscula:  204 até 207
    Letra O Maiuscula:  210 até 214
    Letra U Maiuscula:  217 até 220

    Letra a Minuscula:  224 até 228
    Letra e Minuscula:  232 até 235
    Letra i Minuscula:  236 até 239
    Letra o Minuscula:  242 até 246
    Letra u Minuscula:  249 até 252

    https://marquesfernandes.com/desenvolvimento/codigo-ascii-tabela-ascii-completa/
*/

// depois joga essas funcoes e constante td no header
#define MIN3(a, b, c) ((a) < (b) ? ((a) < (c) ? (a) : (c)) : ((b) < (c) ? (b) : (c)))
#define TAM_MAX_PALAVRA 50  

int hamming(int tamanho, unsigned char *s1, unsigned char *s2) {
    /*  A distância de Hamming entre duas strings de MESMO COMPRIMENTO é
        o número de posições nas quais elas diferem entre si.
        Vista de outra forma, ela corresponde ao menor número de substituições necessárias
        para transformar uma string na outra, ou o número de erros que transformaram uma na outra.
    */
    // https://pt.wikipedia.org/wiki/Distância_de_Hamming#Exemplos


    int distancia = 0;
    for(int i=0; i<tamanho; i++){
        if(*s1!=*s2)distancia++;
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
    // printf("tamanho str 1: %d \n", s1len);
    s2len = strlen(s2);
    // printf("tamanho str 2: %d \n", s2len);
    int column[s1len + 1];
    for (y = 1; y <= s1len; y++)
        column[y] = y;
    for (x = 1; x <= s2len; x++) {
        column[0] = x;
        for (y = 1, lastdiag = x - 1; y <= s1len; y++) {
            olddiag = column[y];
            column[y] = MIN3(column[y] + 1, column[y - 1] + 1, lastdiag + (s1[y-1] == s2[x - 1] ? 0 : 1));
            lastdiag = olddiag;
        }
    }

    // printf("distancia: %d \n", column[s1len] );
    return column[s1len];
}

int testaLetra(unsigned char c) {

    // funcao retorna positivo se o caracterer é válido e
    // negativo se o caractere não for válido

    c = tolower(c);

    if( ( c>=97 && c <=122 ) ||     // alfabeto
    ( c>=224 && c<=228 ) ||         // acentos letra a
    ( c>=232 && c<=235 ) ||         // acentos letra e
    ( c>=236 && c<=239 ) ||         // acentos letra i
    ( c>=242 && c<=246 ) ||         // acentos letra o
    ( c>=249 && c<=252 ) ||         // acentos letra u
    c==231 )                        // cedilha
    return 1;

    else return 0;
}

void testeArquivo(FILE *arquivo){  
    if(!arquivo){
        printf("ERRO AO ABRIR ARQUIVO");
        exit(1);
    }
}

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

size_t achaTamanhoArquivo(FILE *arq){
// funcao para achar a quantidade de linhas do arquivo 
// (so é usada no dicionario para saber a quantidade de palavras)

    fseek(arq, 0, SEEK_SET);

    char c = fgetc(arq);
    size_t tamanho=1;
    while (c!=EOF){
        if(c == '\n')
            tamanho++;
        c = fgetc(arq);
    }
    // printf("qtd de linhas do arquivo: %d", tamanho);
    return tamanho;
}

void leituraDicionarioVETOR(FILE *dic, unsigned char *bancoPalavras[], size_t tamanhoArquivo){
    // alocação e preenchimento do ponteiro com as palavras do dicionario 
    
    fseek( dic, 0, SEEK_SET);

    for(int i=0; i<tamanhoArquivo; i++) {
        bancoPalavras[i] = malloc(sizeof(char) * TAM_MAX_PALAVRA);
        if(!bancoPalavras[i]){
            printf("ERRO AO ALOCAR PONTEIRO %d\n", i+1);
            exit(2);
        }
        fgets(bancoPalavras[i], TAM_MAX_PALAVRA, dic);
        printf("%s", bancoPalavras[i]);
    }

}

int BinarySearch (unsigned char *lista[], unsigned char *busca[], size_t tamanhoDaLista)
{
    int inf = 0;     // limite inferior (o primeiro índice de vetor em C é zero          )
    int sup = tamanhoDaLista-1; // limite superior (termina em um número a menos. 0 a 9 são 10 números)
    int meio;
    while (inf <= sup)
    {
        meio = (inf + sup)/2;
        if (busca[0] == lista[meio][0])
            return meio;
        if (busca[0] < lista[meio][0])
            sup = meio-1;
        else
            inf = meio+1;
    }
    return -1;   // não encontrado
}


// typedef struct no{
//     // nao vou mais usar lista encadeada :(
//     unsigned char palavraDicionario[TAM_MAX_PALAVRA];
//     struct no *proximoNo;  // aponta para a proxima palavra da lista
//     struct no *proximaLetra; // aponta para a primeira palavra que começa com a proxima letra
// } dicionario;

int main(){
setlocale(LC_ALL, "");

clock_t start, end;
start = clock();

FILE *arq = fopen("palavra.txt", "r");
testeArquivo(arq);
// FILE *dic = fopen("dicionario.txt", "r");
// testeArquivo(dicionario);
// FILE *arqCopia = fopen("resposta.txt", "w");
// testeArquivo(arqCopia);


// dicionario *comecoLista = malloc(sizeof(dicionario));   //COMECO DO DICIONARIO NA LISTA ENCADEADA
// leituraDicionarioLISTA(arq, comecoLista);


// PLANO B: SE A LISTA ENCADEADA N DER CERTO EU USO UMA MATRIZ MESMO E VAI NA SORTE 
size_t qtdPalavrasDicionario = achaTamanhoArquivo(arq);
unsigned char *bancoPalavras[qtdPalavrasDicionario];
leituraDicionarioVETOR(arq, bancoPalavras, qtdPalavrasDicionario);



unsigned char *palavra=malloc(sizeof(unsigned char) * TAM_MAX_PALAVRA); // ponteiro para teste de comparação com o dicionario
unsigned char *resetaPalavra = palavra; // ponteiro para voltar para posição original

char letra;     
short int fimDePalavra;
int tamanhoDePalavra;

int totalPalavras = 0;
do{
    tamanhoDePalavra = 0;

    // leitura e teste de caractere
    letra = fgetc(arq);
    fimDePalavra = testaLetra(letra);
    
    // adiciona o caractere e sobe o ponteiro na string contando que seja valido
    if(fimDePalavra){   
    *palavra = letra;
    palavra++;
    tamanhoDePalavra++;
    }

    
    /*  Quando o programa detecta um caractere não valido ele reconhece 
        o conjunto de caracteres anteriores como uma palavra.
        Então o ponteiro volta para a primeira posição e usa 
        as funções hamming e levenshtein
    */

    else{ 
    memset( palavra, '\0', strlen(palavra) - tamanhoDePalavra );  // preenche o resto da string com \0
    palavra = resetaPalavra;    // volta string para posicao original para comparar com o dicionario
    totalPalavras++;
    puts(palavra);
        // IMPLEMENTAR FUNCAO PRA PERCORRER DICIONARIO

        // if(strlen(palavra) != strlen());
        // IMPLEMENTAR FUNCOES HAMMING E LEVENSHTEIN
    palavra = resetaPalavra;    // volta string para posicao original para armazenar uma nova palavra
    }
    }while(letra!=EOF);

/*
PROVAVELMENTE VOU PRECISAR CRIAR OUTRO DO WHILE DENTRO DESSE PARA LER 
CADA PALAVRA SEPARADA E USAR O DE FORA PARA LER O ARQUIVO INTEIRO.

FAÇA UM TESTE PEQUENO DE BUSCA USANDO STRNCMP E PALAVRA.TXT, INICIALMENTE VOU USAR SELECTION SORT MAS
DEPOIS VOU USAR BUSCA BINARIA PARA SER MAIS RAPIDO. POSSO CRIAR UMA FUNCAO SINTETICA PARECIDA COM O
STRNCMP MAS QUE TENHA UM BREAK NO MEIO DA STRING CASO NÃO SEJAM IGUAIS PARA DEIXAR O PROGRAMA MAIS RAPIDO.

CRIE UMA VARIAVEL CONTADORA PARA SABER O TAMANHO DA PALAVRA PARA FACILITAR A COMPARAÇÃO.
*/


printf("\ntotal de palavras: %d", totalPalavras);

// fcloseall();    // funcao que fecha todos os arquivos, não sei se é confiavel
fclose(arq);
// fclose(dic);
// fclose(arqCopia);

end = clock();
double tempo_duracao_programa = (double) (end - start) / CLOCKS_PER_SEC;
printf("\nTEMPO DE DURACAO DO PROGRAMA: %lf", tempo_duracao_programa);


return 0;

}