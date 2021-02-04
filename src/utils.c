/*
 *
 */

#include <stdbool.h>

// Read a bit from a word
bool read_bit(unsigned char word, int bit_number)
{
	return (word >> bit_number) & 1;
}

// Set value of a bit in a word to 1
void set_bit(unsigned char *word, int bit_number)
{
	*word |= (1 << bit_number);
}

// Set value of a bit in a word to 0
void clear_bit(unsigned char *word, int bit_number)
{
	*word &= ~(1 << bit_number);
}