#include <stdio.h>
#include <stdlib.h>
#include "quadtree.h"
#include "treat_pgm_file.h"
#include "bit_manipulation.h"

/**
 * @brief Initializes the bit stream allowing to write bits to a file.
 * 
 * @param file Output file
 */
BitStream *init_bit_stream(FILE *file) {
    if (!file) return NULL;
    
    BitStream *bs = malloc(sizeof(BitStream));
    if (!bs) return NULL;
    
    bs->file = file;
    bs->buffer = 0;
    bs->bit_count = 0;
    return bs;
}

/**
 * @brief Writes a specified number of bits into the bit stream.
 * 
 * @param curr Pointer to the bit stream
 * @param data Contains the bits to write
 * @param nb_bits Contains the number of bits to write
 */
void pushbits(BitStream *curr, unsigned char data, int nb_bits) {
    while (nb_bits > 0) {
        curr->buffer = (curr->buffer << 1) | ((data >> (nb_bits-1)) & 1);
        curr->bit_count++;        
        if (curr->bit_count == 8) { // Write if buffer full
            fwrite(&curr->buffer, 1, 1, curr->file);
            curr->buffer = 0;
            curr->bit_count = 0;
        }
        nb_bits--;
    }
}

/**
 * @brief Reads a specified number of bits from the stream
 * 
 * @param curr Pointer to the bit stream
 * @param nb_bits Contains the number of bits to read
 */
unsigned char pullbits(BitStream *curr, int nb_bits) {
    unsigned char res = 0;    
    while (nb_bits > 0) {
        if (curr->bit_count == 0) {
            if (fread(&curr->buffer, 1, 1, curr->file) != 1) {
                return 0;
            }
            curr->bit_count = 8;
        }        
        res = (res << 1) | ((curr->buffer >> (curr->bit_count - 1)) & 1);
        curr->bit_count--;
        nb_bits--;
    }
    return res;
}

/**
 * @brief Writes remaining bits in the buffer into the file
 * 
 * @param curr Pointer to the bit stream
 */
void finalize_bit_stream(BitStream *curr) {
    if (curr->bit_count > 0) {
        curr->buffer <<= (8 - curr->bit_count);
        fwrite(&curr->buffer, 1, 1, curr->file);
        curr->buffer = 0;
        curr->bit_count = 0;
    }
}

/**
 * @brief Frees the bit stream
 * 
 * @param curr Pointer to the bit stream
 */
void close_bit_stream(BitStream *curr) {
    if (curr) {
        finalize_bit_stream(curr);
        free(curr);
    }
}
