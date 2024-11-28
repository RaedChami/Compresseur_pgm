#include <stdio.h>
#include <stdlib.h>

unsigned char** construit_pixmap(int nbc, int nbl) {
    /**
     * @brief Alloue la mémoire pour la création d'une pixmap.
     * @param nbc Nombre de colonnes du fichier PGM
     * @param nbl Nombre de lignes du fichier PGM
     * @return Renvoie une pixmap correspondant aux dimensions en paramètres 
     */
    unsigned char** pixmap = (unsigned char**)malloc(nbl * sizeof(unsigned char*));      // Allocation de mémoire pour la matrice de pixels
    unsigned char* pixels = (unsigned char*)malloc(nbl * nbc * sizeof(unsigned char));   // Allocation de mémoire pour pixels de la matrice
    
    if (!pixmap || !pixels) {
        return NULL;
    }
    for(int i = 0; i < nbl; i++) {
        pixmap[i] = pixels + (i * nbc);
    }

    return pixmap;
}

void liberer_image(unsigned char** pixmap) {
    /**
     * @brief Libère la mémoire d'une pixmap.
     * @param pixmap Matrice de pixels d'un fichier PGM
     */
    if (pixmap) {
        free(pixmap[0]); 
        free(pixmap);    
    }
}

int lire_fichier(const char *filename, int *nbc, int *nbl, int *nbg, unsigned char ***pixmap) {
    /**
     * @brief Fonction de lecture de fichier de type PGM.
     * @param filename Nom du fichier PGM
     * @param nbc Nombre de colonnes du fichier PGM
     * @param nbl Nombre de lignes du fichier PGM
     * @param pixmap Matrice de pixels du fichier PGM
     * @return Valeur de retour numérique
     */
    FILE *f = fopen(filename, "r");
    char line[1024];
    if (!f) {
        fprintf(stderr, "Erreur d'ouverture du fichier PGM\n");
        return -1;
    }
    if (fgets(line, sizeof(line), f) && (line[0] != 'P' || (line[1] != '5' && line[1] != '2'))) {  // Contrôle du format du fichier
        fprintf(stdout, "Format du fichier doit être P5 ou P2\n");
        fclose(f);
        return -1;
    }

    while (fgets(line, sizeof(line), f)) {
        if (line[0] == '\n') continue;  // Ignore les lignes vides        
        if (line[0] == '#') continue;  // Ignore les commentaires
        
        if (sscanf(line, "%d %d %d", nbc, nbl, nbg) == 3) {   // Identifie les dimensions de l'image et le niveau de gris écrites sur une ligne
            break;
        }
        
        if (sscanf(line, "%d %d", nbc, nbl) == 2) {     // Identifie les dimensions de l'image et le niveau de gris écrites sur deux lignes
            if (fgets(line, sizeof(line), f)) {
                if (sscanf(line, "%d", nbg) == 1) {
                    break;
                }
            }
        }
    }    

    *pixmap = construit_pixmap(*nbc, *nbl);
    fread((*pixmap)[0], 1, (*nbc) * (*nbl), f); // Lis les données binaires du fichier PGM dans une pixmap.
    
    fclose(f);
    return 1;
}

void getDimensions(int nbc, int nbl) {
    printf("Dimensions : %d %d\n", nbc, nbl);
}

void getGrayLevel(int nbg) {
    printf("Niveau de Gris : %d\n", nbg);
}
