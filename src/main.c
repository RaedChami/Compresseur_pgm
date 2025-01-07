#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "traitementPGM.h"
#include "quadtree.h"

void affichage_aide() {
    /**
     * @brief Affiche la page d'aide.
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

void parser_arguments(int argc, char *argv[], int *mode_encodeur, int *mode_decodeur, char **input_file, char **output_file, int *grille_segmentation, int *verbose) {
    /**
     * @brief Extrait les options entrés en argument par l'utilisateur.
     * 
     */
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-c") == 0) {
            *mode_encodeur = 1;
        } else if (strcmp(argv[i], "-u") == 0) {
            *mode_decodeur = 1;
        } else if (strcmp(argv[i], "-i") == 0 && i + 1 < argc) {
            *input_file = argv[i + 1];
            i++;  
        } else if (strcmp(argv[i], "-o") == 0 && i + 1 < argc) {
            *output_file = argv[i + 1];
            i++;  
        } else if (strcmp(argv[i], "-g") == 0) {
            *grille_segmentation = 1;
        } else if (strcmp(argv[i], "-h") == 0) {
            affichage_aide();
            exit(0);
        } else if (strcmp(argv[i], "-v") == 0) {
            *verbose = 1;
        } else {
            fprintf(stderr, "Option inconnue: %s\n", argv[i]);
            affichage_aide();
            exit(1);
        }
    }
}

void verifier_options(int mode_encodeur, int mode_decodeur, const char *input_file) {
    /**
     * @brief Vérifie l'option spécifiée par l'utilisateur
     * 
     * @param mode_encodeur Flag mode encodeur
     * @param mode_decodeur Flag mode decodeur
     * @param input_file Fichier d'entrée
     */
    if (mode_encodeur && mode_decodeur) {
        fprintf(stderr, "Erreur: les options -c et -u sont mutuellement exclusives.\n");
        exit(1);
    }

    if (!input_file) {
        fprintf(stderr, "Erreur: le fichier d'entrée doit être spécifié avec -i.\n");
        exit(1);
    }
}

void determiner_fichier_sortie(int mode_encodeur, int mode_decodeur, char **output_file) {
    /**
     * @brief Determine le fichier de sortie
     * 
     * @param mode_encodeur Flag mode encodeur
     * @param mode_decodeur Flag mode decodeur
     * @param output_file Fichier de sortie 
     */
    if (!*output_file) {
        if (mode_encodeur) {
            *output_file = "QTC/out.qtc";
        } else if (mode_decodeur) {
            *output_file = "PGM/out.pgm";
        }
    }
}

void encoder(const char *input_file, const char *output_file, int verbose) {
    /**
     * @brief Compresse l'image fournie.
     * 
     */
    unsigned char *image = NULL;
    int nbc, nbl, nbg;
    
    if (lire_fichier(input_file, &nbc, &nbl, &nbg, &image) != 1) {
        fprintf(stderr, "Erreur de lecture du fichier\n");
        exit(1);
    }
    
    Noeud *quadtree = construire_quadtree(image, nbc, nbl, 0, 0, nbc);
    if (verbose) {
        afficher_premiers_pixels(image, nbc, nbl);  // Affiche les premiers pixels pour vérification *****DEBUG******
        afficher_quadtree(quadtree, 0);
    }
    
    FILE *f_out = fopen(output_file, "w");
    if (f_out == NULL) {
        fprintf(stderr, "Erreur d'ouverture du fichier de sortie\n");
        free(image);
        exit(1);
    }
    
    printf("Compression de %s vers %s\n", input_file, output_file);

    fclose(f_out);
    free(image);
}

void decoder(const char *input_file, const char *output_file) {
    /**
     * @brief Décompresse l'image fournie.
     * 
     */
    printf("Décompression de %s vers %s\n", input_file, output_file);
}

int main(int argc, char *argv[]) {
    int mode_encodeur = 0;
    int mode_decodeur = 0;
    char *input_file = NULL;
    char *output_file = NULL;
    int grille_segmentation = 0;
    int verbose = 0;

    parser_arguments(argc, argv, &mode_encodeur, &mode_decodeur, &input_file, &output_file, &grille_segmentation, &verbose);
    verifier_options(mode_encodeur, mode_decodeur, input_file);
    determiner_fichier_sortie(mode_encodeur, mode_decodeur, &output_file);

    if (mode_encodeur) {
        encoder(input_file, output_file, verbose);
    } else if (mode_decodeur) {
        decoder(input_file, output_file);
    }

    return 0;
}
