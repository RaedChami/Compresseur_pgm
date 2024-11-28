#include <stdio.h>
#include <stdlib.h>

typedef struct noeud {
    unsigned char m;
    int u;
    int e;
} Noeud;

typedef struct {
    int niveaux;
    int taille_totale;
    Noeud* noeuds;
} Quadtree;
