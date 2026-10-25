# TP-STR-GL4

Travaux pratiques du module **Systèmes Temps-Réel (STR)** -- INSAT, GL4/RT4.

Synchronisation de threads en C avec **POSIX** (`pthread`) : mutex, exclusion mutuelle, variables de condition et moniteurs.

## Groupe

- Slama Amr
- Ben Yarou Adem
- Djebbi Mohamed Nadhir
- Ayed AbdelHakim

**Enseignante :** Olfa Mosbahi

## Sommaire

| TP                                                  | Sujet                                     | Dossier       |
| --------------------------------------------------- | ----------------------------------------- | ------------- |
| [TP1](#tp1--le-pont-en-posix)                       | Le Pont en POSIX                          | [`TP1/`](TP1) |
| [TP2](#tp2--exclusion-mutuelle-sous-posix-le-metro) | Exclusion Mutuelle sous POSIX -- Le Métro | [`TP2/`](TP2) |

---

## TP1 -- Le Pont en POSIX

Un pont supporte une charge maximale de **15 tonnes**. Il est traversé par des camions (15 t) et des voitures (5 t), modélisés chacun par un thread. Le pont est un **moniteur** (mutex + variable(s) de condition) qui garantit que la charge maximale n'est jamais dépassée.

- **Q1** -- moniteur de base : respecte uniquement la capacité du pont.
- **Q2** -- extension avec **priorité aux camions** lorsqu'un camion et une voiture sont en attente (deux variables de condition, deux compteurs d'attente).

```
TP1/
|-- Q1/
|   |-- pont.h
|   |-- voiture.h
|   |-- camion.h
|   `-- main.c
`-- Q2/
    |-- pont.h
    |-- voiture.h
    |-- camion.h
    `-- main.c
```

```bash
cd TP1/Q1   # ou TP1/Q2
gcc main.c -o main -pthread
./main
```

Compte rendu complet : [`TP1/compte_rendu.pdf`](TP1/compte_rendu.pdf).

---

## TP2 -- Exclusion Mutuelle sous POSIX : Le Métro

Un réseau de 6 stations (A, B, C, D, E, F) relié par 3 lignes de métro :

- **MR1** : A -> B -> C -> D (segments AB, BC, CD)
- **MR2** : F -> B -> A (segments BF, AB)
- **MR3** : E -> C -> B (segments EC, BC)

Chaque métro est un thread, chaque segment est protégé par un mutex : un segment ne peut être occupé que par un seul métro à la fois, pendant 2 secondes. Les segments **AB** et **BC** sont partagés entre plusieurs lignes et constituent les points de contention du réseau.

```
TP2/
|-- version1/           # solution avec horodatage ([Xs] sur chaque ligne)
|   |-- main.c
|   `-- log+explication.txt
`-- plus generique/      # routes et metros deduits automatiquement
    |-- main.c
    `-- log+explication.txt
```

```bash
cd "TP2/version1"   # ou "TP2/plus generique"
gcc main.c -o main -pthread
./main
./main | grep "segment AB"   # verifier l'exclusion sur un segment
```

Compte rendu complet .

---

## Prérequis

- `gcc` avec la bibliothèque `pthread`
- Linux / macOS / WSL / MSYS2 (Windows)

## Notes

- L'ordre d'exécution affiché dépend de l'ordonnanceur du système : il peut varier d'une exécution à l'autre. Les propriétés garanties (capacité jamais dépassée, exclusion mutuelle sur chaque segment) restent vraies à chaque exécution.
- Chaque TP détaille sa démarche, son code et l'analyse des simulations dans son compte rendu PDF.
