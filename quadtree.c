#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "quadtree.h"

Noeud *alloue_noeud(unsigned char valeur, unsigned char erreur, bool uniforme) {
    Noeud *noeud = (Noeud*)malloc(sizeof(Noeud));
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

Noeud *construire_quadtree(unsigned char *pixels, int largeur, int hauteur, int x, int y, int taille) {
    if (taille == 1) {     
        // Ajout de noeud au cas de base
        return alloue_noeud(pixels[x + y * largeur], 0, true);      
    }

    Noeud *premier = construire_quadtree(pixels, largeur, hauteur, x, y, taille / 2);
    Noeud *second = construire_quadtree(pixels, largeur, hauteur, x + taille / 2, y, taille / 2);
    Noeud *troisieme = construire_quadtree(pixels, largeur, hauteur, x, y + taille / 2, taille / 2);
    Noeud *quatrieme = construire_quadtree(pixels, largeur, hauteur, x + taille / 2, y + taille / 2, taille / 2);

    bool uniforme = false;

    unsigned char valeurs[4] = {premier->valeur, second->valeur, troisieme->valeur, quatrieme->valeur};
    unsigned char moyenne = (valeurs[0] + valeurs[1] + valeurs[2] + valeurs[3]) / 4;
    unsigned char erreur = (valeurs[0] + valeurs[1] + valeurs[2] + valeurs[3]) % 4;
    if (!erreur) {
        uniforme = ((premier->u && second->u && troisieme->u && quatrieme->u) && (premier->valeur == second->valeur && premier->valeur == troisieme->valeur && premier->valeur == quatrieme->valeur));  
        // Codage du bit d'uniformité lorsque e = 0
    }

    Noeud *noeud_parent = alloue_noeud(moyenne, erreur, uniforme);
    if (noeud_parent) {
        noeud_parent->premier = premier;
        noeud_parent->second = second;
        noeud_parent->troisieme = troisieme;
        noeud_parent->quatrieme = quatrieme;
    }

    return noeud_parent;
}
