#ifndef BIT_MANIPULATION_H
#define BIT_MANIPULATION_H

typedef struct {
    FILE *file;              
    unsigned char buffer;    
    int bit_count;          
} BitStream;

BitStream *init_bit_stream(FILE *f);
void pushbits(BitStream *curr, unsigned char data, int bit_count);
unsigned char pullbits(BitStream *curr, int nb_bits);
void finalize_bit_stream(BitStream *curr);
void close_bit_stream(BitStream *curr);

#endif
