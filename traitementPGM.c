#include <stdio.h>
#include <stdlib.h>

int lire_fichier(const char *filename, int *nbc, int *nbl) {
    /**
     * @brief Foncion de lecture de fichiers de type PGM
     * @param filename Nom du fichier PGM
     * @return valeur de retour de type entier
     */
    FILE *f = fopen(filename, "r");
    char code[100];
    char line[1024];
    if (!f) {
        fprintf(stderr, "Erreur d'ouverture du fichier PGM\n");
        return -1;
    }
    if (fgets(code, sizeof(code), f) != NULL && code[0] != 'P' && code[1] != '5') {     // Contrôle du format du fichier
        fprintf(stdout, "Format du fichier doit être P5 (données binaires)\n");
        fclose(f);
        return -1;
    }
    int nbg;
    if (fgets(line, sizeof(line), f)) {     //  Identification du nombre de colonnes, de lignes et de niveaux de gris du fichier
        if (fscanf(f, "%d %d %d", nbc, nbl, &nbg) != 3) {
          return -1;
        }
    }
    while (fgets(line, sizeof(line), f)) {
        char *debutDeLigne = line;
        if (*debutDeLigne == '#') {     // Ignore les commentaires du fichier
            continue;
        }
//        fprintf(stdout, "%s", line);
    }
    fclose(f);
    return 1;

}
