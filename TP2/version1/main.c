#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>

pthread_mutex_t AB = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t BC = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t CD = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t BF = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t EC = PTHREAD_MUTEX_INITIALIZER;

struct timespec debut;

typedef struct {
    int ligne;         //line: 1 for MR1, 2 for MR2, 3 for MR3
    int num;           //number mtaa el metro on the line (1, 2, ...)
} Metro;

double temps_ecoule(void)
{
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    return (now.tv_sec - debut.tv_sec) + (now.tv_nsec - debut.tv_nsec) / 1e9;
}

void traverse(Metro *metro, pthread_mutex_t *segment, char *nom)
{
    printf("[%5.2fs] MR%d-%d attend le segment %s\n", temps_ecoule(), metro->ligne, metro->num, nom);
    pthread_mutex_lock(segment);
    printf("[%5.2fs] MR%d-%d occupe le segment %s\n", temps_ecoule(), metro->ligne, metro->num, nom);
    sleep(2);
    printf("[%5.2fs] MR%d-%d libere le segment %s\n", temps_ecoule(), metro->ligne, metro->num, nom);
    pthread_mutex_unlock(segment);
}

void *thread_metro(void *arg)
{
    Metro *metro = arg;

    if (metro->ligne == 1) {
        traverse(metro, &AB, "AB");
        traverse(metro, &BC, "BC");
        traverse(metro, &CD, "CD");
    }
    if (metro->ligne == 2) {
        traverse(metro, &BF, "BF");
        traverse(metro, &AB, "AB");
    }
    if (metro->ligne == 3) {
        traverse(metro, &EC, "EC");
        traverse(metro, &BC, "BC");
    }
    return NULL;
}

int main(void)
{
    pthread_t t[6];
    Metro metros[6] = { {1, 1}, {1, 2}, {2, 1}, {2, 2}, {3, 1}, {3, 2} };

    clock_gettime(CLOCK_MONOTONIC, &debut);

    for (int i = 0; i < 6; i++)
        pthread_create(&t[i], NULL, thread_metro, &metros[i]);

    for (int i = 0; i < 6; i++)
        pthread_join(t[i], NULL);

    return 0;
}