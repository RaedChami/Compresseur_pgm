#include <stdio.h>
#include <stdlib.h>
#include "quadtree.h"
#include "bit_manipulation.h"
#include "treat_qtc_file.h"

/**
 * @brief Reads bits from the compressed file and builds the correspondent quadtree by level
 * 
 * @param node Node of quadtree
 * @param bs Bit stream used for reading the bits
 * @param target_lvl Target level of quadtree
 * @param curr_lvl Current browsed level of quadtree
 * @param max_level Maximum level of quadtree 
 */
void read_by_level(Node* node, BitStream* bs, int target_lvl, int curr_lvl, int max_level) {
    if (node == NULL) {
        return;
    }
        
    if (curr_lvl == target_lvl - 1) {               
        if (node->u) {  // If uniformed node, rebuild its identical four childs
            for (int i = 0; i < 4; i++) {
                node->childs[i] = allocate_node(node->value, 0, true);
            }
            return;
        }        
        unsigned char m1 = 0, m2 = 0, m3 = 0;
        for (int i = 0; i < 4; i++) {
            unsigned char value;
            unsigned char e = 0;
            bool u = false;
            if (i < 3) {
                value = pullbits(bs, 8);                
                if (target_lvl < max_level - 1) {
                    e = pullbits(bs, 2);
                    if (e == 0) {
                        u = pullbits(bs, 1);
                    }
                } else { // uniform bit should be true at terminal level
                    u = true;
                }
                
                if (i == 0) m1 = value;
                else if (i == 1) m2 = value;
                else if (i == 2) m3 = value;
            } else { // Reconstruction of 4th child by interpolation
                value = (4 * node->value + node->e) - (m1 + m2 + m3);  // Interpolation formula            
                if (target_lvl < max_level - 1) {
                    e = pullbits(bs, 2);                    
                    if (e == 0) {
                        u = pullbits(bs, 1);
                    }
                } else { // uniform bit should be true at terminal level
                    u = true;
                }
            }
            node->childs[i] = allocate_node(value, e, u);
        }
        return;
    }
           
    for (int i = 0; i < 4; i++) {
        if (node->childs[i]) {
            read_by_level(node->childs[i], bs, target_lvl, curr_lvl + 1, max_level);
        }
    }
}

/**
 * @brief Rebuilds the quadtree from reading the input file.
 * 
 * @param filename Compressed .QTC file
 */
Node* rebuild_quadtree(const char* filename, int verbose) {
    FILE* f = fopen(filename, "rb");
    if (!f) {
        perror("Erreur d'ouverture du fichier");
        exit(1);
    }

    int n, p;
    char compression_date[100];
    if (read_qtc_file(filename, &n, &p, compression_date) != 1) {
        fprintf(stderr, "Erreur de lecture du fichier\n");
        fclose(f);
        exit(1);
    }

    ignore_header(f);

    BitStream* bs = init_bit_stream(f);
    if (!bs) {
        fclose(f);
        exit(1);
    }

    int max_level = p + 1;
    unsigned char root_value = pullbits(bs, 8);
    unsigned char root_error = pullbits(bs, 2);
    bool root_u = false;
    if (verbose) {
        printf("\033[32m==============================\n");     
        printf("Début d'écriture du fichier décompressé...\n");        
        printf("Écriture du niveau maximal: %d\n", max_level - 1);
        printf("Écriture valeur racine: %d\n", root_value);
        printf("Écriture erreur racine: %d\n", root_error);
        printf("....\n");
        printf("==============================\033[0m\n");     
    }
    if (!root_error) {
        root_u = pullbits(bs, 1);
    }
    Node* root = allocate_node(root_value, root_error, root_u);  // Stock the root data into quadtree root
    if (!root) {
        close_bit_stream(bs);
        fclose(f);
        fprintf(stderr, "Erreur lors de l'allocation de la racine.\n");
        exit(1);
    }

    for (int lvl = 1; lvl < max_level; lvl++) { // Rebuilds the quadtree by level after root
        read_by_level(root, bs, lvl, 0, max_level);
    }

    close_bit_stream(bs);
    fclose(f);
    return root;
}