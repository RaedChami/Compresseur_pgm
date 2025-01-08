#include <stdio.h>
#include <stdlib.h>
#include "quadtree.h"
#include "traitementPGM.h"
#include "bit_writer.h"

BitStream *init_bit_stream(FILE *file) {
    /**
     * @brief Initializes the bit stream allowing to write bits to a file.
     * 
     * @param file Output file
     */
    if (!file) return NULL;
    
    BitStream *bs = malloc(sizeof(BitStream));
    if (!bs) return NULL;
    
    bs->file = file;
    bs->buffer = 0;
    bs->bit_count = 0;
    return bs;
}

void pushbits(BitStream *curr, unsigned char data, int nb_bits) {
    /**
     * @brief Writes a specified number of bits into the bit stream.
     * 
     * @param curr Pointer to the bit stream
     * @param data Contains the bits to write
     * @param nb_bits Contains the number of bits to write
     */
    while (nb_bits > 0) {
        curr->buffer = (curr->buffer << 1) | ((data >> (nb_bits-1)) & 1);
        curr->bit_count++;        
        if (curr->bit_count == 8) {
            fwrite(&curr->buffer, 1, 1, curr->file);
            curr->buffer = 0;
            curr->bit_count = 0;
        }
        nb_bits--;
    }
}

void finalize_bit_stream(BitStream *curr) {
    /**
     * @brief Writes remaining bits in the buffer into the file
     * 
     * @param curr Pointer to the bit stream
     */
    if (curr->bit_count > 0) {
        curr->buffer <<= (8 - curr->bit_count);
        fwrite(&curr->buffer, 1, 1, curr->file);
        curr->buffer = 0;
        curr->bit_count = 0;
    }
}

void close_bit_stream(BitStream *curr) {
    /**
     * @brief Frees the bit stream.
     * 
     * @param curr Pointer to the bit stream
     */
    if (curr) {
        finalize_bit_stream(curr);
        free(curr);
    }
}