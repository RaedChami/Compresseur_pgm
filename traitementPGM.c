#include <stdio.h>
#include <stdlib.h>

int lire_fichier(const char *filename, int *nbc, int *nbl, int *nbg) {
    /**
     * @brief Fonction de lecture de fichiers de type PGM
     * @param filename Nom du fichier PGM
     * @return Valeur de retour de type entier
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
            fclose(f);
            return 1;
        }
        
        if (sscanf(line, "%d %d", nbc, nbl) == 2) {     // Identifie les dimensions de l'image et le niveau de gris écrites sur deux lignes
            if (fgets(line, sizeof(line), f)) {
                if (sscanf(line, "%d", nbg) == 1) {
                    fclose(f);
                    return 1;
                }
            }
        }
    }    
    fclose(f);
    return -1;
}

void getDimensions(int nbc, int nbl) {
    printf("Dimensions : %d %d\n", nbc, nbl);
}

void getGrayLevel(int nbg) {
    printf("Niveau de Gris : %d\n", nbg);
}
