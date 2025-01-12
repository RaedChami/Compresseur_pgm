#ifndef DECOMPRESS_H
#define DECOMPRESS_H

void read_by_level(Node* noeud, BitStream* bs, int target_lvl, int curr_lvl, int max_level);
Node* rebuild_quadtree(const char* filename, int verbose);

#endif