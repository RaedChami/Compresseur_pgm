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
    if (!noeud) return NULL;
    noeud->valeur = valeur;
    noeud->e = erreur;
    noeud->u = uniforme;
    noeud->premier = NULL;
    noeud->second = NULL;
    noeud->troisieme = NULL;
    noeud->quatrieme = NULL;
    return noeud;
}

unsigned char calcule_erreur(unsigned char *valeurs) {
    /**
     * @brief Calcule la valeur d'erreur du noeud.
     * @param valeurs Valeurs des noeuds
     * @return Renvoie valeur d'erreur du noeud
     */
    int somme = valeurs[0] + valeurs[1] + valeurs[2] + valeurs[3];
    return somme % 4;
}
