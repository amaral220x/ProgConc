#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>

#define NTHREADS 10

pthread_mutex_t mutex;
pthread_cond_t cond_escr;
pthread_cond_t cond_leit;

int x=0;
int escritores = 0;
int leitores = 0;

void EntraLeitura(int id){
    pthread_mutex_lock(&mutex);
    printf("A thread %d, está lendo...\n",id);
    while(escritores > 0){
        pthread_cond_wait(&cond_leit, &mutex);

    }
    leitores++;
    pthread_mutex_unlock(&mutex);
}

void SaiLeitura(){
    pthread_mutex_lock(&mutex);
    leitores--;
    if(leitores==0)
        pthread_cond_signal(&cond_escr);
    pthread_mutex_unlock(&mutex);
}

void EntraEscrita(int id){
    pthread_mutex_lock(&mutex);
    printf("A thread %d está escrevendo...\n", id);
    while((escritores<0) || (leitores>0 )){
        pthread_cond_wait(&cond_escr, &mutex);
    }
    escritores++;
    pthread_mutex_unlock(&mutex);
}

void SaiEscrita(){
    pthread_mutex_lock(&mutex);
    escritores--;
    pthread_cond_signal(&cond_escr);
    pthread_cond_broadcast(&cond_leit);
    pthread_mutex_unlock(&mutex);
}

void *t1(void *arg){
    int id = *(int *)arg;
    while(1){
        EntraEscrita(id);
        x++;
        printf("T%d: NOVO VALOR DE X= %d\n", id, x);
        SaiEscrita();
        sleep(1);   
    }
    pthread_exit(NULL);
}

void *t2(void *arg){
    int id = *(int *)arg;
    while(1){
        EntraLeitura(id);
        x%2? printf("T%d %d: Impar\n",id, x):printf("T%d %d: Par\n",id, x) ;
        SaiLeitura();
        sleep(1);
    }

    pthread_exit(NULL);
}

void *t3(void *arg){
    int id = *(int *)arg;
    while(1){
        EntraLeitura(id);
        printf("T%d: %d\n",id, x);
        int i =0;
        while(i < 1000) i++;
        SaiLeitura();
        EntraEscrita(id);
        x = id;
        printf("T%d: NOVO VALOR DE X= %d\n", id, x);
        SaiEscrita();
        sleep(1);
    }
    pthread_exit(NULL);
}

int main(void){
    pthread_t *tid;
    int *id;


    tid = (pthread_t *) malloc(sizeof(pthread_t)*NTHREADS);
    id = (int *) malloc(sizeof(int)*NTHREADS);
    //Iniciando variaveis de exclusao mutua
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond_escr, NULL);
    pthread_cond_init(&cond_leit, NULL);

    for(int i=0; i<NTHREADS; i++){
        id[i]= i+0;
        if(i<NTHREADS/3){
            if(pthread_create(tid+i, NULL, t1, (void *) &id[i])) exit(-1);
        }
        else if(i<NTHREADS*2/3){
            if(pthread_create(tid+i, NULL, t2, (void *) &id[i])) exit(-1);
        }
        else{
            if(pthread_create(tid+i, NULL, t3, (void *) &id[i])) exit(-1);
        }

    }

    for(int i=0; i<NTHREADS; i++){
        pthread_join(*(tid+i), NULL);
    }
    printf("FIM\n");
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond_escr);
    pthread_cond_destroy(&cond_leit);
    return 0;
}