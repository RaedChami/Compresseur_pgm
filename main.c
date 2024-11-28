#include <stdio.h>
#include <stdlib.h>
#include "traitementPGM.h"

int main(int argc, char *argv[]) {
    int nbc, nbl, nbg;    
    unsigned char **image = NULL;

    if (argc != 2) {
        fprintf(stderr, "Entrez un fichier en argument\n");
        return -1;
    } 
    lire_fichier(argv[1], &nbc, &nbl, &nbg, &image);
    getDimensions(nbc, nbl);
    getGrayLevel(nbg);
    liberer_image(image);
    return 0;
} 
