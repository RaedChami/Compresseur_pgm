#include <stdio.h>
#include <stdlib.h>
#include "traitementPGM.h"
#include "quadtree.h"

int main(int argc, char *argv[]) {
    unsigned char *image = NULL;
    int nbc, nbl, nbg;

    if (argc != 2) {
        fprintf(stderr, "Entrez un fichier en argument\n");
        return -1;
    } 
    lire_fichier(argv[1], &nbc, &nbl, &nbg, &image);
    getDimensions(nbc, nbl);
    getGrayLevel(nbg);
    Noeud *quadtree = construire_quadtree(image, nbc, nbl, 0, 0, nbc);
    free(image);

    return 0;
}
