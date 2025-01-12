#include <stdio.h>
#include <stdlib.h>
#include "quadtree.h"
#include <time.h>

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

/**
 * @brief Rebuilds the pixmap from the quadtree.
 * 
 * @param node Node of quadtree
 * @param pixmap Matrix of pixels stocked from the quadtree
 * @param x X coordinate of pixel
 * @param y Y coordinate of pixel
 * @param block_size Size of the block to be stocked in pixmap
 * @param total_size Total size of image
 */
void rebuild_pixmap(Node* node, unsigned char* pixmap, int x, int y, int block_size, int total_size) {
    if (!node) return;
    
    if (block_size == 1 || node->u) { // If single pixel or uniformed node, stock the node to pixmap
        for (int i = 0; i < block_size; i++) {
            for (int j = 0; j < block_size; j++) {
                pixmap[(y + i) * total_size + (x + j)] = node->value;
            }
        }
    } else {    // Divide the block of pixels into four
        int new_size = block_size / 2;
        rebuild_pixmap(node->childs[0], pixmap, x, y, new_size, total_size);
        rebuild_pixmap(node->childs[1], pixmap, x + new_size, y, new_size, total_size);
        rebuild_pixmap(node->childs[2], pixmap, x + new_size, y + new_size, new_size, total_size);
        rebuild_pixmap(node->childs[3], pixmap, x, y + new_size, new_size, total_size);
    }
}

/**
 * @brief Writes the header of a decompressed .PGM file
 * 
 * @param f Output file to be written
 * @param size Size of image
 * @param time_compression Compression time saved from .QTC file 
 */
void write_header_decompressed(FILE* f, int size, char* time_compression, int verbose) {
    time_t now = time(NULL);
    fprintf(f, "P5\n");
    fprintf(f, "# Time of compression: %s\n", time_compression);
    fprintf(f, "# Time of decompression: %s", ctime(&now));
    if (verbose) printf("Heure de décompression : %s", ctime(&now));
    fprintf(f, "%d %d\n", size, size);
    fprintf(f, "255\n");
}

/**
 * @brief Rebuilds the .pgm image by writing the header and the data of pixmap into the .pgm output file 
 * 
 * @param root Root of quadtree
 * @param output_filename Decompressed .PGM file
 * @param input_filename Compressed .QTC file
 * @param size Size of image to be rebuilt
 * @param time_compression Time of compression of input file
 */
void rebuild_image(Node* root, const char* output_filename, const char* input_filename, int size, char* time_compression, int verbose) {
    unsigned char* pixmap = malloc(size * size);
    if (!pixmap) {
        return;
    }
    rebuild_pixmap(root, pixmap, 0, 0, size, size);
    FILE* f = fopen(output_filename, "w");
    if (!f) {
        fprintf(stderr, "Erreur d'ouvertude de fichier\n");
        free(pixmap);
        return;
    }
    write_header_decompressed(f, size, time_compression, verbose);
    fwrite(pixmap, 1, size * size, f);
    fclose(f);
    free(pixmap);
}
