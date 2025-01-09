#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "quadtree.h"

Node *allocate_node(unsigned char new_value, unsigned char error, bool _uniform) {
    /**
     * @brief Allocates a node for the quadtree.
     * @param new_value Average of intensity
     * @param error Epsilon/Error value
     * @param _uniform Bit of uniformity
     */
    Node *node = (Node*)malloc(sizeof(Node));
    if (!node) return NULL;
    node->value = new_value;
    node->e = error;
    node->u = _uniform;
    for(int i = 0; i < 4; i++) {
        node->childs[i] = NULL;
    }
    return node;
}

Node *build_quadtree(unsigned char *pixels, int width, int height, int x, int y, int size) {
    /**
     * @brief Builds a quadtree representing the pixels of an image.
     * @param pixels Pixmap
     * @param width Number of columns
     * @param height Number of lines
     * @param x X coordinate of pixel
     * @param y Y coordinate of pixel
     * @param size Size of block of pixels
     */
    bool _uniform = false;

    if (size == 1) {     
        return allocate_node(pixels[x + y * width], 0, true);    // Creates a terminal node
    }

    // Divides a block of pixels into four childs.
    Node *childs[4];
    childs[0] = build_quadtree(pixels, width, height, x, y, size / 2);
    childs[1] = build_quadtree(pixels, width, height, x + size / 2, y, size / 2);
    childs[2] = build_quadtree(pixels, width, height, x + size / 2, y + size / 2, size / 2);
    childs[3] = build_quadtree(pixels, width, height, x, y + size / 2, size / 2);

    unsigned char values[4] = {childs[0]->value, childs[1]->value, childs[2]->value, childs[3]->value};
    unsigned char average = (values[0] + values[1] + values[2] + values[3]) / 4;
    unsigned char epsilon = (values[0] + values[1] + values[2] + values[3]) % 4;
    if (!epsilon) {        // bit coding of uniformity when epsilon = 0
        _uniform = ((childs[0]->u && childs[1]->u && childs[2]->u && childs[3]->u) && (childs[0]->value == childs[1]->value && childs[0]->value == childs[2]->value && childs[0]->value == childs[3]->value));  
    }

    Node *parent_node = allocate_node(average, epsilon, _uniform);
    if (parent_node) {
        for (int i = 0; i < 4; i++) {
            parent_node->childs[i] = childs[i];
        }
    }

    return parent_node;
}

void display_quadtree(Node *node, int level) {
    /**
     * @brief Simple display of the entire quadtree.
     * 
     * @param node Node of quadtree
     * @param level Level of quadtree
     */
    if (!node) {
        return;
    }
    for (int i = 0; i < level; i++) {
        printf("  ");
    }
    printf("Niveau %d --- m: %d, u: %s, e: %d\n", level, node->value, node->u ? "oui" : "non", node->e);
    for (int i = 0; i < 4; i++) {
        display_quadtree(node->childs[i], level + 1);
    }
}

int get_quadtree_depth(Node *node) {
    /**
     * @brief Getter of the depth of the quadtree.
     * Determines and returns the depth of the quadtree
     * 
     * @param node Node of quadtree
     */
    if (!node) {
        return 0; 
    }
    int max_depth = 0;
    for (int i = 0; i < 4; i++) {
        int child_depth = get_quadtree_depth(node->childs[i]);
        if (child_depth > max_depth) {
            max_depth = child_depth;
        }
    }
    return 1 + max_depth;
}
