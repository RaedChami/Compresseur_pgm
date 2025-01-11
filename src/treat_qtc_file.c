#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Verifies, reads and extracts data from a .qtc file
 * @param filename .qtc file
 * @param n Size of expected quadtree
 * @param p Depth of expected quadtree
 * @param time_compression Compression time of .qtc file
 */
int read_qtc_file(const char *filename, int *n, int *p, char *time_compression) {
    FILE *f = fopen(filename, "r");
    char line[2048];
    if (!f) {
        fprintf(stderr, "Erreur d'ouverture du fichier QTC\n");
        return -1;
    }
    if (fgets(line, sizeof(line), f) && (line[0] != 'Q' || (line[1] != '1'))) {   // Verifies format of file
        fprintf(stdout, "Format du fichier doit être Q1\n");
        fclose(f);
        return -1;
    }
    int first_comment = 0;
    while (fgets(line, sizeof(line), f)) {
        if (line[0] == '\n') continue;  // Ignores empty lines
        if (line[0] == '#') {
            if (!first_comment) {
                int i = 0;
                while (line[i] != '\n' && line[i] != '\0') {
                    time_compression[i] = line[i]; // Retrieve compression time
                    i++;
                }
                time_compression[i] = '\0';  
                first_comment = 1;
            }
            continue;  // Ignores rest of comments
        }    
        (*p) = line[0]; // Retrieve encoded size from file
        break;
    }
    (*n) = 2;
    for (int i = 1; i < (*p); i++) { // Determines the depth of the expected quadtree
        (*n) *= 2;
    }
    fclose(f);
    return 1;
}

/**
 * @brief Ignores the header of a .qtc file
 * 
 */
void ignore_header(FILE* f) {
    int c;
    while ((c = fgetc(f)) != EOF && c != '\n');    
    while ((c = fgetc(f)) != EOF) {
        if (c == '#') {
            while ((c = fgetc(f)) != EOF && c != '\n');
        } else if (c == '\n') {
            continue;
        } else {
            fseek(f, -1, SEEK_CUR);
            break;
        }
    }
    fgetc(f);   // Ignores depth encoded value from file    
}
