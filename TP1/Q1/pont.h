#ifndef PONT_H
#define PONT_H

#include <pthread.h>

#define CHARGE_MAX 15

typedef struct {
    int chargeActuelle;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} Pont;

void pont_init(Pont *pont);
void pont_destroy(Pont *pont);
void pont_acceder(Pont *pont, int poids, const char *nomVehicule);
void pont_liberer(Pont *pont, int poids, const char *nomVehicule);

#endif