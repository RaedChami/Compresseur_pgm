/**
 * @brief Calculates and returns the compression rate of an image.
 * 
 */
double calculate_compression_rate(Node *root, int max_level, int verbose) {
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
    if (verbose) {
        printf("Nombre de champs m encodés :%d\n", values_counter);
        printf("Nombre de champs e encodés:%d\n", error_counter);
        printf("Nombre de champs u encodés:%d\n", uniform_counter);
        printf("Taille : %d\n", size * size);
        printf("Taux de compression : %.2f%%\n", compression_rate);
    }
    return compression_rate;
}

/**
 * @brief Writes the header and the compressed data of an image into the file
 * 
 * @param f Written output file 
 * @param root Root of quadtree
 * @param bs Bit stream used for writing the bits
 */
void write_quadtree_data(FILE *f, Node *root, BitStream *bs, int verbose) {
    if (root == NULL) return;

    int max_level = get_quadtree_depth(root);
    if (verbose) {
        display_quadtree(root, 0);
        printf("Écriture du niveau maximal: %d\n", max_level - 1);
        printf("Écriture valeur racine: %d\n", root->value);
        printf("Écriture erreur racine: %d\n", root->e);
    }    
    double compression_rate = calculate_compression_rate(root, max_level, verbose);
    write_header_compressed(f, compression_rate);
    
    pushbits(bs, max_level - 1, 8);
    pushbits(bs, root->value, 8);
    pushbits(bs, root->e, 2);

    for (int level = 1; level < max_level; level++) { // Write data of all levels from the root
        int child_index = 0;
        write_by_level(root, bs, level, 0, max_level, &child_index);
    }

}
