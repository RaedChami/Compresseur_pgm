#include <stdio.h>
#include <stdlib.h>
#include "traitementPGM.h"

int main(int argc, char *argv[]) {
    int nbc, nbl, nbg;    
    unsigned char *pixmap = NULL;

    if (argc != 2) {
        fprintf(stderr, "Entrez un fichier en argument\n");
        return -1;
    } 
    lire_fichier(argv[1], &nbc, &nbl, &nbg, &pixmap);
    getDimensions(nbc, nbl);
    getGrayLevel(nbg);
    
    free(pixmap);
    return 0;
} 
