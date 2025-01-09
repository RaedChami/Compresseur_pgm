#include <stdio.h>
#include <stdlib.h>

int read_pgm_file(const char *filename, int *nbc, int *nbl, int *nbg, unsigned char **pixmap) {
    /**
     * @brief Verifies, reads and extracts data from a .pgm file
     * @param filename .pgm file 
     * @param nbc Number of columns of the .pgm file
     * @param nbl Number of lines of the .pgm file
     * @param pixmap Matrix of pixels built from data of .pgm file
     */
    FILE *f = fopen(filename, "r");
    char line[1024];
    if (!f) {
        fprintf(stderr, "Erreur d'ouverture du fichier PGM\n");
        return -1;
    }
    if (fgets(line, sizeof(line), f) && (line[0] != 'P' || (line[1] != '5'))) {  // Verifies format of file
        fprintf(stdout, "Format du fichier doit être P5\n");
        fclose(f);
        return -1;
    }

    while (fgets(line, sizeof(line), f)) {
        if (line[0] == '\n') continue;  // Ignores empty lines
        if (line[0] == '#') continue;  // Ignores single-line comments

        if (sscanf(line, "%d %d %d", nbc, nbl, nbg) == 3) {   // Identifies dimensions of image and level of gray if in a single-line
            break;
        }
        
        if (sscanf(line, "%d %d", nbc, nbl) == 2) {     // Identifies dimensions of image and level of gray if on two lines
            if (fgets(line, sizeof(line), f)) {
                if (sscanf(line, "%d", nbg) == 1) {
                    break;
                }
            }
        }
    }    

    *pixmap = (unsigned char*)malloc((*nbl) * (*nbc) * sizeof(unsigned char));
    if (!*pixmap) {
        fclose(f);
        return -1;
    }
    fread(*pixmap, sizeof(unsigned char), (*nbc) * (*nbl), f);    // Stocks data into pixmap
    
    fclose(f);
    return 1;
}

// Fonction de debug
void afficher_premiers_pixels(unsigned char *pixmap, int nbc, int nbl) { 
    printf("Premiers pixels de l'image :\n");
    for (int i = 0; i < 20 && i < nbc * nbl; i++) {
        printf("%d ", pixmap[i]);
    }
    printf("\n");
}
