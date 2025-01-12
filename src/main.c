#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include "treat_pgm_file.h"
#include "quadtree.h"
#include "bit_manipulation.h"
#include "compress.h"
#include "treat_qtc_file.h"
#include "decompress.h"

/**
 * @brief Displays the help panel
 * 
 */
void display_help() {
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

/**
 * @brief Parses command-line arguments
 * 
 */
void parse_arguments(int argc, char *argv[], int *encoding_mode, int *decoding_mode, char **input_file, char **output_file, int *segmentation_grid, int *verbose) {
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

/**
 * @brief Validates options specified by user
 * 
 * @param encoding_mode Encoder mode flag
 * @param decoding_mode Decoder mode flag
 * @param input_file Input file
 */
void validate_options(int encoding_mode, int decoding_mode, const char *input_file) {
    if (encoding_mode && decoding_mode) {
        fprintf(stderr, "Erreur: Impossible d'activer l'encodeur et le décodeur en même temps\n");
        exit(1);
    }

    if (!input_file) {
        fprintf(stderr, "Erreur: le fichier d'entrée doit être spécifié avec -i\n");
        exit(1);
    }
}

/**
 * @brief Determines the output file based on the mode.
 * 
 * @param encoding_mode Encoder mode flag
 * @param decoding_mode Decoder mode flag
 * @param input_file Input file
 */
void determine_output_file(int encoding_mode, int decoding_mode, char **output_file) {
    if (!*output_file) {
        if (encoding_mode) {
            *output_file = "QTC/out.qtc";
        } else if (decoding_mode) {
            *output_file = "PGM/out.pgm";
        }
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
        fprintf(stderr, "\033[31mErreur de lecture du fichier\033[0m\n");
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
    printf("\033[0mCompression de %s sauvegardée dans %s\n", input_file, output_file);
    close_bit_stream(bs);
    fclose(f);
    free(image);
}

/**
 * @brief Decompresses the provided QTC file into a PGM file
 * 
 * @param input_file Compressed QTC file
 * @param output_file PGM file
 */
void decode(const char *input_file, const char *output_file, int verbose) {
    FILE *f = fopen(input_file, "rb");
    if (!f) {
        fprintf(stderr, "Erreur d'ouverture de fichier\n");
        exit(1);
    }

    BitStream *bs = init_bit_stream(f);  
    int n, p;
    char time_compression[100];

    if (read_qtc_file(input_file, &n, &p, time_compression) != 1) {
        fprintf(stderr, "\033[31mErreur de lecture du fichier\033[0m\n");
        fclose(f);
        exit(1);
    }   
    Node *quadtree = rebuild_quadtree(input_file, verbose);
    if (!quadtree) {
        fprintf(stderr, "\033[31mErreur de décompression\033[0m\n");
        close_bit_stream(bs);
        fclose(f);
        exit(1);
    }
    rebuild_image(quadtree, output_file, input_file, n, time_compression, verbose);
    printf("Image décompressée sauvegardée dans %s\n", output_file);    
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
        decode(input_file, output_file, verbose);
    }

    return 0;
}
