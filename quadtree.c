#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "quadtree.h"

Noeud *alloue_noeud(unsigned char valeur, unsigned char erreur, bool uniforme) {
    /**
     * @brief Alloue un noeud pour le Quadtree.
     * @param valeur Valeur d'intensité du noeud
     * @param erreur Valeur d'erreur du noeud
     * @param uniforme Bit d'uniformité du noeud
     * @return Renvoie un noeud du Quadtree
     */
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
    /**
     * @brief Construit le quadtree d'une image.
     * @param pixels Matrice de pixels
     * @param largeur Nombre de colonnes de l'image
     * @param hauteur Nombre de lignes de l'image
     * @param x Coordonnée x du pixel
     * @param y Coordonnée y du pixel
     * @param taille Taille du bloc de pixels
     * @return Renvoie noeud parent représentant le bloc de pixels
     */
    if (taille == 1) {     
        return alloue_noeud(pixels[x + y * largeur], 0, true);    // Crée un noeud terminal
    }

    // Divise le bloc de pixels actuel en quatre voisins
    Noeud *premier = construire_quadtree(pixels, largeur, hauteur, x, y, taille / 2);
    Noeud *second = construire_quadtree(pixels, largeur, hauteur, x + taille / 2, y, taille / 2);
    Noeud *troisieme = construire_quadtree(pixels, largeur, hauteur, x + taille / 2, y + taille / 2, taille / 2);
    Noeud *quatrieme = construire_quadtree(pixels, largeur, hauteur, x, y + taille / 2, taille / 2);

    bool uniforme = false;

    unsigned char valeurs[4] = {premier->valeur, second->valeur, troisieme->valeur, quatrieme->valeur};
    unsigned char moyenne = (valeurs[0] + valeurs[1] + valeurs[2] + valeurs[3]) / 4;
    unsigned char erreur = (valeurs[0] + valeurs[1] + valeurs[2] + valeurs[3]) % 4;
    if (!erreur) {        // Codage du bit d'uniformité lorsque e = 0
        uniforme = ((premier->u && second->u && troisieme->u && quatrieme->u) && (premier->valeur == second->valeur && premier->valeur == troisieme->valeur && premier->valeur == quatrieme->valeur));  
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
