#ifndef QUADTREE_H
#define QUADTREE_H
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


typedef struct Noeud {
    unsigned char valeur;
    unsigned char e;    // Valeur d'erreur
    bool u;             // Bit d'uniformité       
    struct Noeud *premier;        
    struct Noeud *second;         
    struct Noeud *troisieme;         
    struct Noeud *quatrieme;         
} Noeud, *Quadtree;

Noeud *alloue_noeud(unsigned char valeur, unsigned char erreur, bool uniforme);
Noeud *construire_quadtree(unsigned char *pixels, int largeur, int hauteur, int x, int y, int taille);
void afficher_quadtree(Noeud *noeud, int niveau);

#endif 