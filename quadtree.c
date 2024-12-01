#include <stdio.h>
#include <stdlib.h>

typedef struct Noeud {
    unsigned char valeur;
    unsigned char e;    // Valeur d'erreur
    bool u;             // Bit d'uniformité       
    struct Noeud *premier;        
    struct Noeud *second;         
    struct Noeud *troisieme;         
    struct Noeud *quatrieme;         
} Noeud, *Quadtree;

Noeud *alloue_noeud(unsigned char valeur, unsigned char erreur, bool uniforme) {
    Noeud *noeud = malloc(sizeof(Noeud));
    noeud->valeur = valeur;
    noeud->e = erreur;
    noeud->u = uniforme;
    noeud->premier = NULL;
    noeud->second = NULL;
    noeud->troisieme = NULL;
    noeud->quatrieme = NULL;
    return noeud;
}
