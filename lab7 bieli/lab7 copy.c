#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define NTHREADS 2
#define N 10

int b[N], ult = 0;
sem_t mutexProd, mutexCons, mutexA, slotCheio, slotVazio;

void Insere(int *item)
{
    static int in = 0;
    sem_wait(&slotVazio);
    sem_wait(&mutexProd);
    printf("Elemento inserido: [");
    for (int i = 0; i < N; i++)
    {
        printf("%d, ", item[i]);
        b[in] = item[i];
        in = (in + 1) % N;
        sem_post(&slotCheio);
    }
    printf("]\n");
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
    }
    pthread_exit(NULL);
}

int main()
{
    pthread_t tid[NTHREADS];
    sem_init(&mutexCons, 0, 1);
    sem_init(&slotVazio, 0, 1);
    sem_init(&slotCheio, 0, 0);
    sem_init(&mutexProd, 0, 1);

    pthread_create(&tid[0], NULL, produtor, NULL);
    pthread_create(&tid[1], NULL, consumidor, NULL);
    // pthread_create(&tid[2], NULL, produtor, NULL);
    // pthread_create(&tid[3], NULL, consumidor, NULL);
    for (int t = 0; t < NTHREADS; t++)
    {
        if (pthread_join(tid[t], NULL))
        {
            printf("--ERRO: pthread_join() \n");
            exit(-1);
        }
    }
    pthread_exit(NULL);
}
