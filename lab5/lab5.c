//Gabriel Vieira do Amaral
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#define NTHREADS 4
//variaveis globais
int estado = 0;
pthread_mutex_t lockEstado;
pthread_cond_t condEstado;

//Criando Threads
void *t4(void* elem){ //A primeira a ser executada. Basta rodar e dar o sinal
    pthread_mutex_lock(&lockEstado);
    printf("Seja bem-vindo\n");
    estado++;
    pthread_cond_signal(&condEstado);
    pthread_mutex_unlock(&lockEstado);
    pthread_exit(NULL);
}

void *t3(void *elem){
    pthread_mutex_lock(&lockEstado);
    if(estado==0){
        while(estado == 0){ //Espero o sinal e libero as outras para esperarem também.
            pthread_cond_wait(&condEstado, &lockEstado);
            pthread_cond_signal(&condEstado);
        }
    }
    estado++;
    printf("Sente-se, por favor.\n");
    pthread_cond_signal(&condEstado);
    pthread_mutex_unlock(&lockEstado);
    pthread_exit(NULL);
}

void *t2(void *elem){
    pthread_mutex_lock(&lockEstado);
    if(estado==0){
        while(estado == 0){//Espero o sinal e libero as outras para esperarem também.
            pthread_cond_wait(&condEstado, &lockEstado);
            pthread_cond_signal(&condEstado);
        }
    }
    estado++;
    printf("Fique a vontade.\n");
    pthread_cond_signal(&condEstado);
    pthread_mutex_unlock(&lockEstado);
    pthread_exit(NULL);
}

void *t1(void *elem){ //Ultima a ser executada. Caso esperado estado == 3
    pthread_mutex_lock(&lockEstado);
    if(estado<3){
        while(estado<3){ //Espero o sinal e libero as outras para esperarem também.
            pthread_cond_wait(&condEstado, &lockEstado);
            pthread_cond_signal(&condEstado);
        }
    }
    estado++;
    printf("Volte sempre!\n");
    pthread_cond_signal(&condEstado);
    pthread_mutex_unlock(&lockEstado);
    pthread_exit(NULL);
}

int main(void){
    pthread_t threads[NTHREADS];

    pthread_mutex_init(&lockEstado, NULL);
    pthread_cond_init(&condEstado, NULL);

    pthread_create(&threads[0], NULL, t1, NULL);
    pthread_create(&threads[1], NULL, t2, NULL);
    pthread_create(&threads[2], NULL, t3, NULL);
    pthread_create(&threads[3], NULL, t4, NULL);

    for(int i=0; i<NTHREADS; i++){
        pthread_join(threads[i], NULL);
    }
    printf("FIM\n");
    pthread_mutex_destroy(&lockEstado);
    pthread_cond_destroy(&condEstado);
    return 0;
}