#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include "treat_pgm_file.h"
#include "quadtree.h"
#include "bit_writer.h"
#include "compress.h"

void display_help() {
    /**
     * @brief Displays the help panel
     * 
     */
    printf("Usage: ./codec [options]\n");
    printf("Options:\n");
    printf("  -c                        Encoder l'image\n");
    printf("  -u                        Decoder l'image\n");
    printf("  -i <input>.{pgm|qtc}      Fichier d'entrée\n");
    printf("  -o <output>.{pgm|qtc}     Renomme le fichier de sortie\n");
    printf("  -g                        Éditer la grille de segmentation\n");
    printf("  -h                        Afficher cette aide\n");
    printf("  -v                        Mode bavard\n");
}

void parse_arguments(int argc, char *argv[], int *encoding_mode, int *decoding_mode, char **input_file, char **output_file, int *segmentation_grid, int *verbose) {
    /**
     * @brief Parses command-line arguments
     * 
     */
    int opt;
    while ((opt = getopt(argc, argv, "hcui:o:gv")) != -1) {
        switch (opt) {
            case 'c':
                *encoding_mode = 1;
                break;
            case 'u':
                *decoding_mode = 1;
                break;
            case 'i':
                *input_file = optarg;
                break;
            case 'o':
                *output_file = optarg;
                break;
            case 'g':
                *segmentation_grid = 1;
                break;
            case 'v':
                *verbose = 1;
                break;
            case 'h':
                display_help();
                exit(0);
            default:
                display_help();
                exit(1);
        }
    }
}

void validate_options(int encoding_mode, int decoding_mode, const char *input_file) {
    /**
     * @brief Validates options specified by user
     * 
     * @param encoding_mode Encoder mode flag
     * @param decoding_mode Decoder mode flag
     * @param input_file Input file
     */
    if (encoding_mode && decoding_mode) {
        fprintf(stderr, "Erreur: Impossible d'activer l'encodeur et le décodeur en même temps\n");
        exit(1);
    }

    if (!input_file) {
        fprintf(stderr, "Erreur: le fichier d'entrée doit être spécifié avec -i\n");
        exit(1);
    }
}

void determine_output_file(int encoding_mode, int decoding_mode, char **output_file) {
    /**
     * @brief Determines the output file based on the mode.
     * 
     * @param encoding_mode Encoder mode flag
     * @param decoding_mode Decoder mode flag
     * @param input_file Input file
     */
    if (!*output_file) {
        if (encoding_mode) {
            *output_file = "QTC/out.qtc";
        } else if (decoding_mode) {
            *output_file = "PGM/out.pgm";
        }
    }
}

/*
    Fonction DEBUG 
*/
void display_info(int verbose, int encoding_mode, const char *input_file, const char *output_file, int segmentation_grid, int nbc, int nbl, int nbg, const unsigned char *image) {
    if (verbose) {
        printf("Mode: %s\n", encoding_mode ? "Encodeur" : "Decodeur");
        printf("Fichier d'entrée: %s\n", input_file);
        printf("Fichier de sortie: %s\n", output_file);
        printf("Grille de segmentation: %s\n", segmentation_grid ? "Oui" : "Non");
        printf("Dimensions : %d %d\n", nbc, nbl);
        printf("Niveau de Gris : %d\n", nbg);
        printf("Premiers pixels de l'image :\n");
        for (int i = 0; i < 20 && i < nbc * nbl; i++) {
            printf("%d ", image[i]);
        }
        printf("\n");
    }
}

/**
 * @brief Compresses the provided PGM image into a QTC file
 * 
 * @param input_file   PGM file to be compressed
 * @param output_file   Compressed QTC file
 */
void encode(const char *input_file, const char *output_file, int verbose) {
    unsigned char *image = NULL;
    int nbc, nbl, nbg;
    
    if (read_pgm_file(input_file, &nbc, &nbl, &nbg, &image) != 1) {
        fprintf(stderr, "Erreur de lecture du fichier\n");
        exit(1);
    }
    
    Node *quadtree = build_quadtree(image, nbc, nbl, 0, 0, nbc);

    FILE *f = fopen(output_file, "w");
    if (f == NULL) {
        fprintf(stderr, "Erreur d'ouverture du fichier de sortie\n");
        free(image);
        exit(1);
    }    
    BitStream *bs = init_bit_stream(f);
    write_quadtree_data(f, quadtree, bs, verbose);
    printf("Compression de %s sauvegardée dans %s\n", input_file, output_file);
    close_bit_stream(bs);
    fclose(f);
    free(image);
}

void decode(const char *input_file, const char *output_file) {
    /**
     * @brief Decompresses the provided QTC file into a PGM file
     * 
     * @param input_file Compressed QTC file
     * @param output_file PGM file
     */
    printf("Décompression de %s vers %s\n", input_file, output_file);
    if (lire_fichier_qtc(input_file) != 1) {
        fprintf(stderr, "Erreur de lecture du fichier\n");
        exit(1);
    }
}

int main(int argc, char *argv[]) {
    int encoding_mode = 0;
    int decoding_mode = 0;
    char *input_file = NULL;
    char *output_file = NULL;
    int segmentation_grid = 0;
    int verbose = 0;

    parse_arguments(argc, argv, &encoding_mode, &decoding_mode, &input_file, &output_file, &segmentation_grid, &verbose);
    validate_options(encoding_mode, decoding_mode, input_file);
    determine_output_file(encoding_mode, decoding_mode, &output_file);

    if (encoding_mode) {
        encode(input_file, output_file, verbose);
    } else if (decoding_mode) {
        decode(input_file, output_file);
    }

    return 0;
}
