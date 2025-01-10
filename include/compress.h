#ifndef COMPRESS_H
#define COMPRESS_H
#include <stdio.h>
#include <stdlib.h>

void write_header(FILE *f, double compression_rate);
void write_quadtree_data(FILE *filename, Node *node, BitStream *bs);
#endif