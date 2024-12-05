#include <stdio.h>
#include <stdlib.h>

int lire_fichier(const char *filename, int *nbc, int *nbl, int *nbg, unsigned char **pixmap) {
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

    *pixmap = (unsigned char*)malloc((*nbl) * (*nbc) * sizeof(unsigned char));   // Allocation de mémoire pour pixels de la matrice
    if (!*pixmap) {
        fclose(f);
        return -1;
    }
    fread(*pixmap, sizeof(unsigned char), (*nbc) * (*nbl), f);    
    
    fclose(f);
    return 1;
}

void getDimensions(int nbc, int nbl) {
    printf("Dimensions : %d %d\n", nbc, nbl);
}

void getGrayLevel(int nbg) {
    printf("Niveau de Gris : %d\n", nbg);
}
