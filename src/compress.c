#include <stdio.h>
#include <stdlib.h>
#include "quadtree.h"
#include "treat_pgm_file.h"
#include "bit_writer.h"
#include <time.h>

void write_header(FILE *f, double compression_rate) {
    /**
     * @brief Writes the header of a compressed .qtc file
     * 
     * @param f Output file to be written
     */
    time_t now = time(NULL);
    fprintf(f, "Q1\n");
    fprintf(f, "# %s", ctime(&now)); 
    fprintf(f, "# compression rate %.2f%%\n", compression_rate);
}

void count_node(Node *node, bool terminal_level, bool last_child, int *values_counter, int *error_counter, int *uniform_counter) {
    /**
     * @brief Counts different useful data from a node of the quadtree
     * 
     * @param node Node of the quadtree
     * @param terminal_level Is the last level of a quadtree
     * @param last_child Is the last child of a node
     * @param values_counter Counts the number of value/average of pixels
     * @param error_counter Counts the number of epsilon/error values
     * @param uniform_counter Counts the number of uniform bit
     */
    if (node == NULL) return;
    if (!last_child) {
        (*values_counter)++;
    }
    if (!terminal_level) {
        (*error_counter)++;
        if (node->e == 0) {
            (*uniform_counter)++;
        }
    }
}

void count_by_level(Node *root, int target_level, int curr_level, int max_level, int *child_index, int *values_counter, int *error_counter, int *uniform_counter) {
    /**
     * @brief Counts the different useful data for the header from all levels of the quadtree
     * @param root Root of the quadtree
     * @param target_level Target level of quadtree
     * @param curr_level Current browsed level of quadtree
     * @param max_level Maximum level of quadtree
     * @param child_index Index of browsed child node
     * @param values_counter Counts the number of value/average of pixels
     * @param error_counter Counts the number of epsilon/error values
     * @param uniform_counter Counts the number of uniform bit 
     */
    if (root == NULL) return;
    if (curr_level == target_level) {
        bool terminal_level = (target_level == max_level - 1);
        bool last_child = (*child_index == 3);
        count_node(root, terminal_level, last_child, values_counter, error_counter, uniform_counter);
        *child_index = (*child_index + 1) % 4;
        return;
    }
    if (!root->u) {
        for (int i = 0; i < 4; i++) {
            count_by_level(root->childs[i], target_level, curr_level + 1, max_level, child_index, values_counter, error_counter, uniform_counter);
        }
    }
}

void write_node(Node *node, BitStream *bs, bool terminal_level, bool last_child) {
    /**
     * @brief Writes the data of a node into the bit stream.
     * 
     * @param node Node of quadtree
     * @param bs Bit stream used for writing the bits
     * @param terminal_level Is the last level of a quadtree
     * @param last_child Is the last child of a node
     */
    if (node == NULL) return;

    if (!last_child) {
        printf("valeur: %d\n", node->value);
        pushbits(bs, node->value, 8);
    }

    if (!terminal_level) {
        printf("e: %d\n", node->e);
        pushbits(bs, node->e, 2);

        if (node->e == 0) {
            printf("u: %d\n", node->u);
            pushbits(bs, node->u, 1);
        }
    }
}

void write_by_level(Node *root, BitStream *bs, int target_level, int curr_level, int max_level, int *child_index) {
    /**
     * @brief Writes the data of all nodes from all levels of the quadtree
     * 
     * @param root Root of quadtree
     * @param bs Bit stream used for writing the bits
     * @param target_level Target level of quadtree
     * @param curr_level Current browsed level of quadtree
     * @param max_level Maximum level of quadtree
     * @param child_index Index of browsed child node 
     */
    if (root == NULL) return;

    if (curr_level == target_level) {
        bool terminal_level = (target_level == max_level - 1);
        bool last_child = (*child_index == 3);
        write_node(root, bs, terminal_level, last_child);       
        *child_index = (*child_index + 1) % 4;
        return;
    }

    if (!root->u) {
        for (int i = 0; i < 4; i++) {
            write_by_level(root->childs[i], bs, target_level, curr_level + 1, max_level, child_index);
        }
    }
}

double calculate_compression_rate(Node *root, int max_level) {
    /**
     * @brief Calculates and returns the compression rate of an image.
     * 
     */
    int values_counter = 1;  
    int error_counter = 1;  
    int uniform_counter = 0;
    int size = 1;
    for (int i = 1; i < max_level; i++) { // Determines the size of the quadtree
        size *= 2;
    }

    for (int level = 1; level < max_level; level++) {  // Counts the useful data for the header from all levels of the quadtree 
        int child_index = 0;
        count_by_level(root, level, 0, max_level, &child_index, &values_counter, &error_counter, &uniform_counter);
    }

    int encoded_data = values_counter * 8 + error_counter * 2 + uniform_counter * 1; 
    double compression_rate = (double)encoded_data / ((size * size) * 8) * 100;    // Compression rate formula
    printf("m :%d\n", values_counter);
    printf("e :%d\n", error_counter);
    printf("u :%d\n", uniform_counter);
    printf("taille :%d\n", size * size);
    printf("%.2f%%\n", compression_rate);
    return compression_rate;
}

void write_quadtree_data(FILE *f, Node *root, BitStream *bs) {
    /**
     * @brief Writes the header and the compressed data of an image into the file
     * 
     * @param f Written output file 
     * @param root Root of quadtree
     * @param bs Bit stream used for writing the bits
     */
    if (root == NULL) return;

    int max_level = get_quadtree_depth(root);
    double compression_rate = calculate_compression_rate(root, max_level);
    write_header(f, compression_rate);
    
    printf("Écriture du niveau maximal: %d\n", max_level - 1);
    pushbits(bs, max_level - 1, 8);
    printf("Écriture valeur racine: %d\n", root->value);
    pushbits(bs, root->value, 8);
    printf("Écriture erreur racine: %d\n", root->e);
    pushbits(bs, root->e, 2);
    for (int level = 1; level < max_level; level++) { // Write data of all levels from the root
        printf("\nniveau %dn", niveau);
        int child_index = 0;
        write_by_level(root, bs, level, 0, max_level, &child_index);
    }
}
