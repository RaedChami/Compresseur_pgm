#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "quadtree.h"

Noeud *alloue_noeud(unsigned char valeur, unsigned char erreur, bool _uniforme) {
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
    noeud->u = _uniforme;
    for(int i = 0; i < 4; i++) {
        noeud->fils[i] = NULL;
    }
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
    bool _uniforme = false;

    if (taille == 1) {     
        return alloue_noeud(pixels[x + y * largeur], 0, true);    // Crée un noeud terminal
    }

    // Divise le bloc de pixels en quatre voisins
    Noeud *fils[4];
    fils[0] = construire_quadtree(pixels, largeur, hauteur, x, y, taille / 2);
    fils[1] = construire_quadtree(pixels, largeur, hauteur, x + taille / 2, y, taille / 2);
    fils[2] = construire_quadtree(pixels, largeur, hauteur, x + taille / 2, y + taille / 2, taille / 2);
    fils[3] = construire_quadtree(pixels, largeur, hauteur, x, y + taille / 2, taille / 2);

    unsigned char valeurs[4] = {fils[0]->valeur, fils[1]->valeur, fils[2]->valeur, fils[3]->valeur};
    unsigned char moyenne = (valeurs[0] + valeurs[1] + valeurs[2] + valeurs[3]) / 4;
    unsigned char erreur = (valeurs[0] + valeurs[1] + valeurs[2] + valeurs[3]) % 4;
    if (!erreur) {        // Codage du bit d'uniformité lorsque e = 0
        _uniforme = ((fils[0]->u && fils[1]->u && fils[2]->u && fils[3]->u) && (fils[0]->valeur == fils[1]->valeur && fils[0]->valeur == fils[2]->valeur && fils[0]->valeur == fils[3]->valeur));  
    }

    Noeud *noeud_parent = alloue_noeud(moyenne, erreur, _uniforme);
    if (noeud_parent) {
        for (int i = 0; i < 4; i++) {
            noeud_parent->fils[i] = fils[i];
        }
    }

    return noeud_parent;
}

void afficher_quadtree(Noeud *noeud, int niveau) {
    if (!noeud) {
        return;
    }
    for (int i = 0; i < niveau; i++) {
        printf("  ");
    }
    printf("Niveau %d --- m: %d, u: %s, e: %d\n", niveau, noeud->valeur, noeud->u ? "oui" : "non", noeud->e);
    for (int i = 0; i < 4; i++) {
        afficher_quadtree(noeud->fils[i], niveau + 1);
    }
}
