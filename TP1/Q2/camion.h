#ifndef CAMION_H
#define CAMION_H

#include "pont.h"

typedef struct {
    char nom[50];
    Pont *pont;
} CamionArgs;

void *runner_camion(void *arg);

#endif