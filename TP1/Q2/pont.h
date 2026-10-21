#ifndef PONT_H
#define PONT_H

#include <pthread.h>

#define CHARGE_MAX 15

typedef struct {
    int chargeActuelle;
    int nbCamionsEnAttente;   // Nombre de camions bloqués en attente
    int nbVoituresEnAttente;  // Nombre de voitures bloquées en attente
    pthread_mutex_t mutex;
    pthread_cond_t cond_camion;  // File d'attente prioritaire pour les camions
    pthread_cond_t cond_voiture; // File d'attente pour les voitures
} Pont;

void pont_init(Pont *pont);
void pont_destroy(Pont *pont);
void pont_acceder(Pont *pont, int poids, const char *nomVehicule);
void pont_liberer(Pont *pont, int poids, const char *nomVehicule);

#endif