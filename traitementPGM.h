#ifndef TRAITEMENTPGM_H
#define TRAITEMENTPGM_H
#include <stdio.h>
#include <stdlib.h>

unsigned char* construit_pixmap(int nbc, int nbl);
int lire_fichier(const char *filename, int *nbc, int *nbl, int *nbg, unsigned char **pixmap);
void getDimensions(int nbc, int nbl);
void getGrayLevel(int nbg);

#endif
