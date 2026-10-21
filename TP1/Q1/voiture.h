#ifndef VOITURE_H
#define VOITURE_H

#include "pont.h"

typedef struct {
    char nom[50];
    Pont *pont;
} VoitureArgs;

void *runner_voiture(void *arg);

#endif