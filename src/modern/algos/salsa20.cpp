#include "salsa20.h"
#include "endianness.h"
#include <string.h>

#define rotate_left(a,b) (((a) << (b)) | ((a) >> (32 - (b))))
#define quarter_round(a, b, c, d)(		\
	b ^= rotate_left(a + d, 7),	\
	c ^= rotate_left(b + a, 9),	\
	d ^= rotate_left(c + b,13),	\
	a ^= rotate_left(d + c,18))
#define ROUNDS 20
 

static void salsa20_block(u32 x[16])
{
	u32 array_copy[16];
    
    // If we're on a little endian machine, nothing to be done.
    // If we're on a big endian machine, we have to shift our perspective as though
    // we were on a little endian machine.
    // Little endian machines sort of do this https://stackoverflow.com/q/7184789/9731532
    // when shifting bits over bytes. Big endian machines do not do this. This is why
    // conversion between big endianness and little endianness is invertible under
    // addition and shifting, but I'm not sure if that's the right way to say it.
    // It has similar vibes to the identity in linear algebra that has to do with eigenvectors:
    // https://youtu.be/P2LTAUO1TdA?t=718

#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
    for (int i = 0; i < 16; i++)
    {
        x[i] = u32_toggle_endianness(x[i]);
        array_copy[i] = x[i];
    }
#else
    memcpy(array_copy, x, 16 * 4);
#endif

	// 10 loops × 2 rounds/loop = 20 rounds
	for (int i = 0; i < ROUNDS; i += 2) 
    {
		// Odd round
		quarter_round(x[ 0], x[ 4], x[ 8], x[12]);	// column 1
		quarter_round(x[ 5], x[ 9], x[13], x[ 1]);	// column 2
		quarter_round(x[10], x[14], x[ 2], x[ 6]);	// column 3
		quarter_round(x[15], x[ 3], x[ 7], x[11]);	// column 4
		// Even round
		quarter_round(x[ 0], x[ 1], x[ 2], x[ 3]);	// row 1
		quarter_round(x[ 5], x[ 6], x[ 7], x[ 4]);	// row 2
		quarter_round(x[10], x[11], x[ 8], x[ 9]);	// row 3
		quarter_round(x[15], x[12], x[13], x[14]);	// row 4
	}

	for (int i = 0; i < 16; i++)
    {
#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
		x[i] = u32_toggle_endianness(x[i] + array_copy[i]);
#else
        x[i] += array_copy[i];
#endif
    }

}

static void salsa20_expand16(u8 key[16], u8 nonce_and_stream_index[16], u8 keystream[64])
{
    // The constants specified by the Salsa20 specification, 'tau'
    // "expand 16-byte k"
    u8 tau[4][4] = {
        { 'e', 'x', 'p', 'a' },
        { 'n', 'd', ' ', '1' },
        { '6', '-', 'b', 'y' },
        { 't', 'e', ' ', 'k' }
    };

    /*
        Copy all of 'tau' into the correct spots in our keystream block
        
        By words, it looks like this (indices are shown):

            0  -  -  -
            -  5  -  -
            -  -  10 -
            -  -  -  15
        
        By bytes, like this:
        
            0  1  2  3  | -  -  -  -  | -  -  -  -  | -  -  -  -
            -  -  -  -  | 20 21 22 23 | -  -  -  -  | -  -  -  -
            -  -  -  -  | -  -  -  -  | 40 41 42 43 | -  -  -  -
            -  -  -  -  | -  -  -  -  | -  -  -  -  | 60 61 62 63
    */
    for (int i = 0; i < 4; i ++)
    {
        for (int j = 0; j < 4; j++)
            keystream[i * 20 + j] = tau[i][j];
    }
    /*
        Copy the key and the nonce into the keystream block.

        non is nonce
        pos is stream index

        they are concatened together in the input array.

        tau  key  key  key
        key  tau  non  non
        pos  pos  tau  key
        key  key  key  tau    

        The key positions:

        -  1  2  3
        4  -  -  -
        -  -  -  11
        12 13 14 -

        The nonce_and_stream_index positions:

        -  -  -  -
        -  -  6  7
        8  9  -  -
        -  -  -  -

        To me, this is way clearer with a memcpy.
    */
    memcpy(&keystream[ 1 * 4], key, 4 * 4); 
    memcpy(&keystream[11 * 4], key, 4 * 4); 
    memcpy(&keystream[ 6 * 4], nonce_and_stream_index, 4 * 4);

    salsa20_block((u32*)keystream);
}

// The sole difference is that the 16-byte version copies the key into the keystream twice
// while this one does it only once (for the second 16-byte sequence the second half of the key is used).
static void salsa20_expand32(u8 key[32], u8 nonce_and_stream_index[16], u8 keystream[64])
{
    // The constants specified by the Salsa20 specification, 'sigma', "expand 32-byte k"
    u8 sigma[4][4] = {
        { 'e', 'x', 'p', 'a' },
        { 'n', 'd', ' ', '3' },
        { '2', '-', 'b', 'y' },
        { 't', 'e', ' ', 'k' }
    };

    for (int i = 0; i < 4; i ++)
    {
        for (int j = 0; j < 4; j++)
            keystream[i * 20 + j] = sigma[i][j];
    }

    memcpy(&keystream[ 1 * 4], key,         4 * 4); 
    memcpy(&keystream[11 * 4], &key[4 * 4], 4 * 4); 
    memcpy(&keystream[ 6 * 4], nonce_and_stream_index, 4 * 4);

    salsa20_block((u32*)keystream);
}


static void salsa20_crypt(
    u8* key, u8 nonce[8], decltype(salsa20_expand32) *expand,
    u8 *input_output_buffer, u32 buffer_length)
{
    u32 stream_index = 0;
    u8 keystream[64];
    u8 nonce_and_stream_index[4 * 4];

    memcpy(nonce_and_stream_index, nonce, 8);

    // Since the stream index is a u32 (come on, it's never gonna be larger than that)
    // the upper 4 bytes, reserved for it in this array, will always be zero
    memset(&nonce_and_stream_index[12], 0, 4);

    for (u32 i = stream_index; i < buffer_length; i++) 
    {
        int keystream_index = i % 64;

        // Looped back or just started, gotta refresh the keystream
        if (keystream_index == 0) 
        {
            // Make it so that the least significant byte is the first in the word
            u32 block_number = u32_to_little_endian(i / 64);
            memcpy(&nonce_and_stream_index[8], &block_number, 4);
            (*expand)(key, nonce_and_stream_index, keystream);
        }

        // xor one byte of plaintext with one byte of keystream
        input_output_buffer[i] ^= keystream[keystream_index];
    }
}

void salsa20_crypt16(
    u8 key_16_byte[16], u8 nonce[8], 
    u8 *input_output_buffer, u32 buffer_length)
{
    return salsa20_crypt(key_16_byte, nonce, &salsa20_expand16, input_output_buffer, buffer_length);
}


void salsa20_crypt32(
    u8 key_32_byte[32], u8 nonce[8], 
    u8 *input_output_buffer, u32 buffer_length)
{
    return salsa20_crypt(key_32_byte, nonce, &salsa20_expand32, input_output_buffer, buffer_length);
}