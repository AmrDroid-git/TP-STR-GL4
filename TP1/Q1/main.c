#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#define CHARGE_MAX 15


//how to run the code: 
// gcc main.c -o main
// ./main
// PONT Implementation

typedef struct {
    int chargeActuelle;
    pthread_mutex_t mutex; //declaration de mutex
    pthread_cond_t cond;
} Pont;

void pont_init(Pont *pont) {
    pont->chargeActuelle = 0;
    pthread_mutex_init(&pont->mutex, NULL);
    pthread_cond_init(&pont->cond, NULL);
}

void pont_destroy(Pont *pont) {
    pthread_mutex_destroy(&pont->mutex);
    pthread_cond_destroy(&pont->cond);
}

void pont_acceder(Pont *pont, int poids, const char *nomVehicule) {
    pthread_mutex_lock(&pont->mutex);
    
    // Equivalent of while (chargeActuelle + poids > CHARGE_MAX) wait();
    while (pont->chargeActuelle + poids > CHARGE_MAX) {
        pthread_cond_wait(&pont->cond, &pont->mutex); // make the thread sleep + unlock the mutex
    }
    
    pont->chargeActuelle += poids;
    printf("%s (%dt) est entre. Charge actuelle : %dt\n", nomVehicule, poids, pont->chargeActuelle);
    
    pthread_mutex_unlock(&pont->mutex);
}

void pont_liberer(Pont *pont, int poids, const char *nomVehicule) {
    pthread_mutex_lock(&pont->mutex);
    
    pont->chargeActuelle -= poids;
    printf("%s (%dt) est sorti. Charge actuelle : %dt\n", nomVehicule, poids, pont->chargeActuelle);
    
    // Equivalent of notifyAll() dans javva
    pthread_cond_broadcast(&pont->cond);
    
    pthread_mutex_unlock(&pont->mutex);
}

// VEHICLE THREAD ROUTINES
typedef struct {
    char nom[50];
    Pont *pont;
} VehiculeArgs;

// Voiture thread function
void *runner_voiture(void *arg) {
    VehiculeArgs *args = (VehiculeArgs *)arg;

    pont_acceder(args->pont, 5, args->nom);
    sleep(1); // Temps de traversee (1s)
    pont_liberer(args->pont, 5, args->nom);

    free(args);
    return NULL;
}

// Camion thread function
void *runner_camion(void *arg) {
    VehiculeArgs *args = (VehiculeArgs *)arg;

    pont_acceder(args->pont, 15, args->nom);
    sleep(2); // Temps de traversée (2s)
    pont_liberer(args->pont, 15, args->nom);

    free(args);
    return NULL;
}

// MAIN FUNCTION
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

    // Créeation et démarrage des threads
    pthread_create(&t_v1, NULL, runner_voiture, v1);
    pthread_create(&t_v2, NULL, runner_voiture, v2);
    pthread_create(&t_c1, NULL, runner_camion, c1);
    pthread_create(&t_v3, NULL, runner_voiture, v3);

    // Attente de la fin des threads
    pthread_join(t_v1, NULL);
    pthread_join(t_v2, NULL);
    pthread_join(t_c1, NULL);
    pthread_join(t_v3, NULL);

    pont_destroy(&pont);

    return 0;

    //documentation utilisé
    //https://www.geeksforgeeks.org/c/thread-functions-in-c-c/
}