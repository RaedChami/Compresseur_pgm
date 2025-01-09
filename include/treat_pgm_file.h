#ifndef TREAT_PGM_FILE_H
#define TREAT_PGM_FILE_H
#include <stdio.h>
#include <stdlib.h>

int read_pgm_file(const char *filename, int *nbc, int *nbl, int *nbg, unsigned char **pixmap);
void afficher_premiers_pixels(unsigned char *pixmap, int nbc, int nbl);

#endif
