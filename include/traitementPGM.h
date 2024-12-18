#ifndef TRAITEMENTPGM_H
#define TRAITEMENTPGM_H
#include <stdio.h>
#include <stdlib.h>

int lire_fichier(const char *filename, int *nbc, int *nbl, int *nbg, unsigned char **pixmap);
void getDimensions(int nbc, int nbl);
void getGrayLevel(int nbg);
void afficher_premiers_pixels(unsigned char *pixmap, int nbc, int nbl);

#endif