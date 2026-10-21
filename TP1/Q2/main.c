#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#define CHARGE_MAX 15

// STRUCTURE ET IMPLEMENTATION DU PONT (QUESTION 2)

typedef struct {
    int chargeActuelle;
    int nbCamionsEnAttente;   // Compteur de camions en attente
    int nbVoituresEnAttente;  // Compteur de voitures en attente
    pthread_mutex_t mutex; // pour la fonction IF (charge > 15)
    pthread_cond_t cond_camion;  // File d'attente des camions
    pthread_cond_t cond_voiture; // File d'attente des voitures
} Pont;

void pont_init(Pont *pont) {
    pont->chargeActuelle = 0;
    pont->nbCamionsEnAttente = 0;
    pont->nbVoituresEnAttente = 0;
    pthread_mutex_init(&pont->mutex, NULL);
    pthread_cond_init(&pont->cond_camion, NULL);
    pthread_cond_init(&pont->cond_voiture, NULL);
}

void pont_destroy(Pont *pont) {
    pthread_mutex_destroy(&pont->mutex);
    pthread_cond_destroy(&pont->cond_camion);
    pthread_cond_destroy(&pont->cond_voiture);
}

void pont_acceder(Pont *pont, int poids, const char *nomVehicule) {
    pthread_mutex_lock(&pont->mutex);

    if (poids == 15) { 
        // LOGIQUE POUR UN CAMION (15t)
        pont->nbCamionsEnAttente++;
        
        // Un camion attend si le pont n'est pas complètement vide (chargeActuelle > 0)
        while (pont->chargeActuelle + poids > CHARGE_MAX) {
            pthread_cond_wait(&pont->cond_camion, &pont->mutex);
        }
        
        pont->nbCamionsEnAttente--;
    } else { 
        // LOGIQUE POUR UNE VOITURE (5t)
        pont->nbVoituresEnAttente++;
        
        // REGLE DE PRIORITE :
        // Une voiture doit attendre si le pont est plein (charge > 10t) 
        // OU SI UN CAMION EST EN ATTENTE (priorité absolue aux camions)
        while ((pont->chargeActuelle + poids > CHARGE_MAX) || (pont->nbCamionsEnAttente > 0)) {
            pthread_cond_wait(&pont->cond_voiture, &pont->mutex);
        }
        
        pont->nbVoituresEnAttente--;
    }

    pont->chargeActuelle += poids;
    printf("[ENTREE] %s (%dt) est entre. Charge actuelle : %dt\n", nomVehicule, poids, pont->chargeActuelle);

    pthread_mutex_unlock(&pont->mutex);
}


void pont_liberer(Pont *pont, int poids, const char *nomVehicule) {
    pthread_mutex_lock(&pont->mutex);

    pont->chargeActuelle -= poids;
    printf("[SORTIE] %s (%dt) est sorti. Charge actuelle : %dt\n", nomVehicule, poids, pont->chargeActuelle);


    // Si au moins un camion est en attente ET que le pont s'est entièrement libéré (0t),
    // on réveille en Priorité le camion.
    if (pont->nbCamionsEnAttente > 0 && pont->chargeActuelle == 0) {
        pthread_cond_signal(&pont->cond_camion);
    } else {
        // Sinon, on réveille les voitures en attente
        pthread_cond_broadcast(&pont->cond_voiture);
    }

    pthread_mutex_unlock(&pont->mutex);
}



typedef struct {
    char nom[50];
    Pont *pont;
} VehiculeArgs;

void *runner_voiture(void *arg) {
    VehiculeArgs *args = (VehiculeArgs *)arg;

    pont_acceder(args->pont, 5, args->nom);
    sleep(1); // Temps de traversée (1s)
    pont_liberer(args->pont, 5, args->nom);

    free(args);
    return NULL;
}

void *runner_camion(void *arg) {
    VehiculeArgs *args = (VehiculeArgs *)arg;

    pont_acceder(args->pont, 15, args->nom);
    sleep(2); // Temps de traversée (2s)
    pont_liberer(args->pont, 15, args->nom);

    free(args);
    return NULL;
}



int main() {
    Pont pont;
    pont_init(&pont);

    pthread_t t_v1, t_v2, t_c1, t_v3;

    VehiculeArgs *v1 = malloc(sizeof(VehiculeArgs));
    strcpy(v1->nom, "Voiture 1");
    v1->pont = &pont;

    VehiculeArgs *v2 = malloc(sizeof(VehiculeArgs));
    strcpy(v2->nom, "Voiture 2");
    v2->pont = &pont;

    VehiculeArgs *c1 = malloc(sizeof(VehiculeArgs));
    strcpy(c1->nom, "Camion 1");
    c1->pont = &pont;

    VehiculeArgs *v3 = malloc(sizeof(VehiculeArgs));
    strcpy(v3->nom, "Voiture 3");
    v3->pont = &pont;

    // Lancement des threads
    pthread_create(&t_v1, NULL, runner_voiture, v1);
    pthread_create(&t_v2, NULL, runner_voiture, v2);
    pthread_create(&t_c1, NULL, runner_camion, c1);
    pthread_create(&t_v3, NULL, runner_voiture, v3);

    // Attente des threads
    pthread_join(t_v1, NULL);
    pthread_join(t_v2, NULL);
    pthread_join(t_c1, NULL);
    pthread_join(t_v3, NULL);

    pont_destroy(&pont);

    return 0;
}