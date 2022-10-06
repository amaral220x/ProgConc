#include<stdio.h>
#include<stdlib.h>
#include "timer.h"
#include <time.h>
#include <pthread.h>
#define clear() printf("\033[H\033[J")

float *matriz1, *matriz2, *saida; //matriz que será carregada do arquivo
int linhas1, colunas1, linhas2, colunas2, nthreads, *ids; //dimensoes da matriz
double start, finish, elapsed;
int aux = 0; //Indicar qual thread tá sendo executada.
void *tarefa (void *arg){
    long int id = * (long int *) arg;
    for(int i = id; i < linhas1; i+=nthreads){
        for(int j = 0; j < colunas2; j++){
            for(int k = 0; k < colunas1; k++){
                saida[i*colunas2 + j] += matriz1[i*colunas1 + k] * matriz2[k*colunas2 + j];
            }
        }
    }
    pthread_exit(NULL);
}
int lerMatriz(char nome[], int aux){
    float *matriz;
    int linhas, colunas; //dimensoes da matriz
    long long int tam; //qtde de elementos na matriz
    FILE * descritorArquivo; //descritor do arquivo de entrada
    size_t ret; //retorno da funcao de leitura no arquivo de entrada
    descritorArquivo = fopen(nome, "rb");
    if(!descritorArquivo) {
        fprintf(stderr, "Erro de abertura do arquivo\n");
        return 2;
    }

    //le as dimensoes da matriz
    ret = fread(&linhas, sizeof(int), 1, descritorArquivo);
    if(!ret) {
        fprintf(stderr, "Erro de leitura das dimensoes da matriz arquivo \n");
        return 3;
    }
    ret = fread(&colunas, sizeof(int), 1, descritorArquivo);
    if(!ret) {
        fprintf(stderr, "Erro de leitura das dimensoes da matriz arquivo \n");
        return 3;
    }
    tam = linhas * colunas; //calcula a qtde de elementos da matriz

    //aloca memoria para a matriz
    matriz = (float*) malloc(sizeof(float) * tam);
    if(!matriz) {
        fprintf(stderr, "Erro de alocao da memoria da matriz\n");
        return 3;
    }

    //carrega a matriz de elementos do tipo float do arquivo
    ret = fread(matriz, sizeof(float), tam, descritorArquivo);
    if(ret < tam) {
        fprintf(stderr, "Erro de leitura dos elementos da matriz\n");
        return 4;
    }
    if(aux == 0){
        matriz1 = matriz;
        linhas1 = linhas;
        colunas1 = colunas;
    }
    if(aux){
        matriz2 = matriz;
        linhas2 = linhas;
        colunas2 = colunas;
    }
}
void imprime(int x){ 
    float *matriz;
    int linhas, colunas;
    if(x==0){ //Ifs para indicar qual matriz imprimir e evitar passar parâmetros. (Tava sempre dando seg fault)
        matriz = matriz1;
        linhas = linhas1;
        colunas = colunas1;
        printf("Matriz 1:");
    }
    if(x==1){
        matriz = matriz2;
        linhas = linhas2;
        colunas = colunas2;
        printf("Matriz 2:");
    }
    if(x==2){
        matriz = saida;
        linhas = linhas1;
        colunas = colunas2;      
        printf("Matriz de Saida:");
    }
    fprintf(stdout, "\n");
    for(int i=0; i<linhas; i++) { 
        for(int j=0; j<colunas; j++)
            fprintf(stdout, "%.6f ", matriz[i*colunas+j]);
        fprintf(stdout, "\n");
    }
    fprintf(stdout, "\n");
}
void multiplica(){
    for(int i=0; i<linhas1; i++) { 
        for(int j=0; j<colunas2; j++){
            saida[i*colunas2+j] = 0;
            for(int k=0; k<colunas1; k++)
                saida[i*colunas2+j] += matriz1[i*colunas1+k] * matriz2[k*colunas2+j];
        }
    }
}
int geraArquivo(char nome[]){
    FILE * descritorArquivo; //descritor do arquivo de entrada
    size_t ret; //retorno da funcao de leitura no arquivo de entrada
    descritorArquivo = fopen(nome, "wb");
    if(!descritorArquivo) {
        fprintf(stderr, "Erro de abertura do arquivo\n");
        return 2;
    }
    ret = fwrite(&linhas1, sizeof(int), 1, descritorArquivo);
    if(!ret) {
        fprintf(stderr, "Erro de escrita das dimensoes da matriz arquivo \n");
        return 3;
    }
    ret = fwrite(&colunas2, sizeof(int), 1, descritorArquivo);
    if(!ret) {
        fprintf(stderr, "Erro de escrita das dimensoes da matriz arquivo \n");
        return 3;
    }
    ret = fwrite(saida, sizeof(float), linhas1*colunas2, descritorArquivo);
    if(ret < linhas1*colunas2) {
        fprintf(stderr, "Erro de escrita dos elementos da matriz\n");
        return 4;
    }
}
int main(int argc, char * argv[]){
    pthread_t *tid; //Identificador da Thread
    double total = 0;
    double momento1,momento2,momento3 = 0;
    if(argc == 5){
        ids = (int *) malloc(sizeof(int) * nthreads);
        GET_TIME(start); //Tempo de leitura das matrizes
        lerMatriz(argv[1], 0);
        lerMatriz(argv[2], 1);
        GET_TIME(finish);
        nthreads = atoi(argv[4]);
        if(nthreads > linhas1){ //Max de Threads possível == max de linhas
            nthreads = linhas1;
        }
        tid = (pthread_t * ) malloc(sizeof(pthread_t) * nthreads);
        if(tid == NULL){
            printf("Erro ao alocar memoria para as threads");
            return 2;
        }
        momento1 = finish-start;
        total += finish-start;
        printf("Ate aqui ok\n");
        //imprime(0);
        //imprime(1);
        GET_TIME(start);
        saida = (float *) malloc(sizeof(float) *linhas1*colunas2);
        // Multiplicação concorrente
        if(linhas2 != colunas1){ //Condição de existencia da multiplicação
            printf("Não é possível realizar a multiplicacao \n");
            return 4;
        }
        for(long int i = 0; i<nthreads; i++){
            ids[i] = i;
            if(pthread_create(tid+i, NULL, tarefa, (void *) (ids+i))){
                printf("Erro ao criar threads");
                return 3;
            }
        }
        //Esperando terminar
        for(int i = 0; i<nthreads; i++){
            pthread_join(*(tid+i), NULL);
        }
        //multiplica();
        GET_TIME(finish);
        momento2 = finish-start;
        total += finish-start;
        GET_TIME(start);
        //imprime(2);
        geraArquivo(argv[3]);
        //liberando as coisas
        free(saida);
        free(matriz1);
        free(matriz2);
        GET_TIME(finish);
        momento3 = finish-start;
        total += finish-start;
        //printf("\n\n\nPressione pra continuar\n");
        //getchar();
        //clear();
        printf("\nTempo de leitura das matrizes: %f\n", momento1);
        printf("Tempo de multiplicacao das matrizes: %f\n", momento2);
        printf("Tempo de escrita da matriz e free: %f\n", momento3);
        printf("Tempo total: %f\n", total);
    }
    else
        printf("Por favor, entre com 4 argumentos: <matriz1><matriz2><saida><nthreads>\n");
}