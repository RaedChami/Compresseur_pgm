#include <stdio.h>
#include <stdlib.h>
#include "traitementPGM.h"

int main(int argc, char *argv[]) {
    int nbc, nbl, nbg;    
    if (argc != 2) {
        fprintf(stderr, "Entrez un fichier en argument\n");
        return 1;
    } else {
        lire_fichier(argv[1], &nbc, &nbl, &nbg);
        getDimensions(nbc, nbl);
        getGrayLevel(nbg);
    }  
    return 0;
} 
