#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define N 10
#define C 2
#define P 2

int b[N], ult = 0;
sem_t mutexProd, mutexCons, mutexA, slotCheio, slotVazio;

void dorme(void){
    int i;
    for(i=0; i<100000000; i++);
}

void Insere(int *item)
{
    static int in = 0;
    sem_wait(&slotVazio);
    sem_wait(&mutexProd);
    printf("Elemento inserido: [");
    for (int i = 0; i < N; i++)
    {
        printf("%d, ", item[i]);
        dorme();
        b[in] = item[i];
        in = (in + 1) % N;
    }
    printf("]\n");
    for(int i = 0; i < N; i++)
    {
        sem_post(&slotCheio);
    }
    sem_post(&mutexProd);
}

int Retira(void)
{
    int item;
    static int out = 0;
    sem_wait(&slotCheio);

    sem_wait(&mutexCons);
    item = b[out];
    out = (out + 1) % N;
    if (out == 0)
    { // Chegamos no ultimo
        sem_post(&mutexCons);
        sem_post(&slotVazio);
    }
    else
    {
        sem_post(&mutexCons);
    }
    return item;
}

void *produtor(void *arg)
{
    int elem[N];
    while (1)
    {
        for (int i = 0; i < N; i++)
            elem[i] = i;
        Insere(elem);
    }
    pthread_exit(NULL);
}

void *consumidor(void *arg)
{
    int elem;
    while (1)
    {
        elem = Retira();
        printf("ELEMENTO RETIRADO %d\n", elem);
        dorme();
    }
    pthread_exit(NULL);
}

int main()
{
    pthread_t tid[P+C];
    sem_init(&mutexCons, 0, 1);
    sem_init(&slotVazio, 0, 1);
    sem_init(&slotCheio, 0, 0);
    sem_init(&mutexProd, 0, 1);

    for (int i = 0; i < P; i++)
    {
        if (pthread_create(&tid[i], NULL, produtor, NULL))
        {
            printf("ERRO: pthread_create()\n");
            exit(1);
        }
    }
    for (int i = C; i < P + C; i++)
    {
        if (pthread_create(&tid[i], NULL, consumidor, NULL))
        {
            printf("ERRO: pthread_create()\n");
            exit(1);
        }
    }
    for (int t = 0; t < P+C; t++)
    {
        if (pthread_join(tid[t], NULL))
        {
            printf("--ERRO: pthread_join() \n");
            exit(-1);
        }
    }
    pthread_exit(NULL);
}
