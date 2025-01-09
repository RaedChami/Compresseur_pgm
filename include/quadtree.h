#ifndef QUADTREE_H
#define QUADTREE_H
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct Node {
    unsigned char value;  
    unsigned char e;    // Error value
    bool u;             // Uniformity bit       
    struct Node *childs[4];  // Childs          
} Node, *Quadtree;

Node *allocate_node(unsigned char value, unsigned char error, bool uniform);
Node *build_quadtree(unsigned char *pixels, int width, int height, int x, int y, int size);
void display_quadtree(Node *node, int level);
int get_quadtree_depth(Node *node);

#endif 
