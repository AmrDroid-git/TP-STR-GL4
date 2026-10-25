#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#define NB_SEGMENTS 5

typedef struct {
    char *nom;
    pthread_mutex_t mutex;
} Segment;

Segment segments[NB_SEGMENTS] = {
    { "AB", PTHREAD_MUTEX_INITIALIZER },
    { "BC", PTHREAD_MUTEX_INITIALIZER },
    { "CD", PTHREAD_MUTEX_INITIALIZER },
    { "BF", PTHREAD_MUTEX_INITIALIZER },
    { "EC", PTHREAD_MUTEX_INITIALIZER }
};

char *lignes[] = { "ABCD", "FBA", "ECB" };

Segment *trouver_segment(char a, char b)
{
    for (int i = 0; i < NB_SEGMENTS; i++) {
        char *nom = segments[i].nom;
        if ((nom[0] == a && nom[1] == b) || (nom[0] == b && nom[1] == a))
            return &segments[i];
    }
    return NULL;
}

void traverse(char *metro, Segment *segment)
{
    printf("%s attend le segment %s\n", metro, segment->nom);
    pthread_mutex_lock(&segment->mutex);
    printf("%s occupe le segment %s\n", metro, segment->nom);
    sleep(2);
    printf("%s libere le segment %s\n", metro, segment->nom);
    pthread_mutex_unlock(&segment->mutex);
}

void *thread_metro(void *arg)
{
    char *metro = arg;
    int ligne;

    sscanf(metro, "MR%d", &ligne);
    char *route = lignes[ligne - 1];

    for (int i = 0; route[i + 1] != '\0'; i++)
        traverse(metro, trouver_segment(route[i], route[i + 1]));

    return NULL;
}

int main(void)
{
    char *metros[] = { "MR1-1", "MR1-2", "MR2-1", "MR2-2", "MR3-1", "MR3-2" };
    int n = sizeof(metros) / sizeof(metros[0]);
    pthread_t t[n];

    for (int i = 0; i < n; i++)
        pthread_create(&t[i], NULL, thread_metro, metros[i]);

    for (int i = 0; i < n; i++)
        pthread_join(t[i], NULL);

    return 0;
}