#include "rijndael.h"
#include "endianness.h"
#include <string.h>
#include <assert.h>

static const int key_bits[] = {
    /* AES_CYPHER_128 */ 128,
    /* AES_CYPHER_192 */ 192,
    /* AES_CYPHER_256 */ 256,
};

static const int number_of_rounds[] = {
    /* AES_CYPHER_128 */  10,
    /* AES_CYPHER_192 */  12,
    /* AES_CYPHER_256 */  14,
};

// int state_dimensions_in_bytes[] = {
//     /* AES_CYPHER_128 */  4,
//     /* AES_CYPHER_192 */  4,
//     /* AES_CYPHER_256 */  4,
// };

/*
 * aes Rcon:
 *
 * WARNING: Rcon is designed starting from 1 to 15, not 0 to 14.
 *          FIPS-197 Page 9: "note that i starts at 1, not 0"
 *
 * i    |   0     1     2     3     4     5     6     7     8     9    10    11    12    13    14
 * -----+------------------------------------------------------------------------------------------
 *      | [01]  [02]  [04]  [08]  [10]  [20]  [40]  [80]  [1b]  [36]  [6c]  [d8]  [ab]  [4d]  [9a]
 * RCON | [00]  [00]  [00]  [00]  [00]  [00]  [00]  [00]  [00]  [00]  [00]  [00]  [00]  [00]  [00]
 *      | [00]  [00]  [00]  [00]  [00]  [00]  [00]  [00]  [00]  [00]  [00]  [00]  [00]  [00]  [00]
 *      | [00]  [00]  [00]  [00]  [00]  [00]  [00]  [00]  [00]  [00]  [00]  [00]  [00]  [00]  [00]
 */
static const u32 rcon[] = {
    0x01000000, 0x02000000, 0x04000000, 0x08000000, 0x10000000, 0x20000000, 0x40000000, 0x80000000,
    0x1b000000, 0x36000000, 0x6c000000, 0xd8000000, 0xab000000, 0xed000000, 0x9a000000
};

static const u8 sbox[256] = {
 /* 0     1     2     3     4     5     6     7     8     9     A     B     C     D     E     F  */
    0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
    0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
    0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
    0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
    0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
    0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
    0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
    0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
    0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
    0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
    0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
    0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
    0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
    0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
    0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
    0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16
};

static const u8 inv_sbox[256] = {
 /* 0     1     2     3     4     5     6     7     8     9     A     B     C     D     E     F  */
    0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,
    0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,
    0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
    0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,
    0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,
    0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
    0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,
    0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,
    0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
    0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,
    0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,
    0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
    0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
    0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,
    0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
    0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d
};

inline u8 sub_sbox(u8 val)
{
    return sbox[val];
}

inline u32 sub_sbox_dword(u32 val)
{
    u32 tmp = 0;
   
    tmp |= ((u32)sub_sbox((u8)((val >>  0) & 0xFF))) <<  0;
    tmp |= ((u32)sub_sbox((u8)((val >>  8) & 0xFF))) <<  8;
    tmp |= ((u32)sub_sbox((u8)((val >> 16) & 0xFF))) << 16;
    tmp |= ((u32)sub_sbox((u8)((val >> 24) & 0xFF))) << 24;

    return tmp;
}

inline u32 rotate_dword(u32 val)
{
    u32 tmp = val;
   
    return (val >> 8) | ((tmp & 0xFF) << 24);
}

inline u32 swap_dword(u32 val)
{
    return (((val & 0x000000FF) << 24) |
            ((val & 0x0000FF00) <<  8) |
            ((val & 0x00FF0000) >>  8) |
            ((val & 0xFF000000) >> 24));
}

static void expand_key(AES_CYPHER_T mode, const u8* key, u8* round_keys)
{
    u32* w = (u32*)round_keys;

    // The first iteration just copies the key bytes.
    for (int i = 0; i < rijndael_key_length_in_dwords[mode]; i++)
    {
        w[i] = *(u32*)&key[i * 4];
    }

    for (int i = rijndael_key_length_in_dwords[mode]; i < 4 * (number_of_rounds[mode] + 1); i++)
    {
        u32 t = w[i - 1];

        if (i % rijndael_key_length_in_dwords[mode] == 0)
        {
            t = rotate_dword(t);
            t = sub_sbox_dword(t);
            t = t ^ rcon[i / rijndael_key_length_in_dwords[mode] - 1];
        }
        else if (rijndael_key_length_in_dwords[mode] > 6 && i % rijndael_key_length_in_dwords[mode] == 4)
        {
            t = sub_sbox_dword(t);
        }

        t = t ^ w[i - rijndael_key_length_in_dwords[mode]];
        w[i] = t;
    }
}


inline void sub_bytes(u8 *state)
{
    for (int i = 0; i < 4 * 4; i++) 
    {
        state[i] = sub_sbox(state[i]);
    }
}

static void shift_rows(u8 *state)
{
    u8 *s = (u8*)state;
    int i, j, r;
   
    for (i = 1; i < 4; i++) 
    {
        for (j = 0; j < i; j++) 
        {
            u8 tmp = s[i];
            for (r = 0; r < 3; r++) 
            {
                s[i + r * 4] = s[i + (r + 1) * 4];
            }
            s[i + 3 * 4] = tmp;
        }
    }
}

inline u8 xtime(u8 x)
{
    return (x << 1) ^ ((x >> 7) * 0x1b);
}

inline u8 xtimes(u8 x, int ts)
{
    while (ts-- > 0) 
    {
        x = xtime(x);
    }
   
    return x;
}

static u8 multiply(u8 x, u8 y)
{
    /*
     * encrypt: y has only 2 bits: can be 1, 2 or 3
     * decrypt: y could be any value of 9, b, d, or e
     */
   
    return ((((y >> 0) & 1) * xtimes(x, 0)) ^
            (((y >> 1) & 1) * xtimes(x, 1)) ^
            (((y >> 2) & 1) * xtimes(x, 2)) ^
            (((y >> 3) & 1) * xtimes(x, 3)) ^
            (((y >> 4) & 1) * xtimes(x, 4)) ^
            (((y >> 5) & 1) * xtimes(x, 5)) ^
            (((y >> 6) & 1) * xtimes(x, 6)) ^
            (((y >> 7) & 1) * xtimes(x, 7)));
}

static void mix_columns(u8 *state)
{
    u8 y[16] = { 2, 3, 1, 1,  1, 2, 3, 1,  1, 1, 2, 3,  3, 1, 1, 2 };
    u8 s[4];
   
    for (int i = 0; i < 4; i++) 
    {
        for (int r = 0; r < 4; r++) 
        {
            s[r] = 0;
            for (int j = 0; j < 4; j++) 
            {
                s[r] = s[r] ^ multiply(state[i * 4 + j], y[r * 4 + j]);
            }
        }
        for (int r = 0; r < 4; r++) 
        {
            state[i * 4 + r] = s[r];
        }
    }
}

inline void add_round_key(u8 *state, u8 *round_keys, size_t round)
{
    u32 *w = (u32*)round_keys;
    u32 *s = (u32*)state;
   
    for (size_t i = 0; i < 4; i++) 
    {
        s[i] ^= w[round * 4 + i];
    }
}

void rijndael_encrypt(AES_CYPHER_T mode, const u8* key, 
    u8* input_output_buffer, const size_t buffer_length)
{
    assert(buffer_length % 16 == 0);
    
    u8 round_keys[4 * 4 * 15];
    u8 state[4 * 4];

    expand_key(mode, key, round_keys);

    for (size_t i = 0; i < buffer_length / 16; i++)
    {
        for (size_t j = 0; j < 4 * 4; j++)
        {
            // Assume the length of input is a multiple of 16
            state[j] = input_output_buffer[16 * i + j] ^ round_keys[j];
        }

        for (int round = 1; round < number_of_rounds[mode]; round++)
        {
            sub_bytes(state);
            shift_rows(state);
            mix_columns(state);
            add_round_key(state, round_keys, round);
        }
        
        sub_bytes(state);
        shift_rows(state);
        
        for (size_t j = 0; j < 4 * 4; j++)
        {
            // Assume the length of input is a multiple of 16
            input_output_buffer[16 * i + j] = state[j] ^ round_keys[4 * number_of_rounds[mode] + j];
        }
    }
}

static void inv_shift_rows(u8 *state)
{
    u8 *s = (u8 *)state;
    int i, j, r;
   
    for (i = 1; i < 4; i++) 
    {
        for (j = 0; j < 4 - i; j++) 
        {
            u8 tmp = s[i];
            for (r = 0; r < 4; r++) 
            {
                s[i + r * 4] = s[i + (r + 1) * 4];
            }
            s[i + (3) * 4] = tmp;
        }
    }
}

inline u8 inv_sub_sbox(u8 val)
{
    return inv_sbox[val];
}

static void inv_sub_bytes(u8 *state)
{
    int i, j;
   
    for (i = 0; i < 4; i++) 
    {
        for (j = 0; j < 4; j++) 
        {
            state[i * 4 + j] = inv_sub_sbox(state[i * 4 + j]);
        }
    }
}

static void inv_mix_columns(u8 *state)
{
    u8 y[16] = { 0x0e, 0x0b, 0x0d, 0x09,  0x09, 0x0e, 0x0b, 0x0d,
                 0x0d, 0x09, 0x0e, 0x0b,  0x0b, 0x0d, 0x09, 0x0e };
    u8 s[4];
    int i, j, r;
   
    for (i = 0; i < 4; i++) 
    {
        for (r = 0; r < 4; r++) 
        {
            s[r] = 0;
            for (j = 0; j < 4; j++) 
            {
                s[r] = s[r] ^ multiply(state[i * 4 + j], y[r * 4 + j]);
            }
        }
        for (r = 0; r < 4; r++) 
        {
            state[i * 4 + r] = s[r];
        }
    }
}

void rijndael_decrypt(AES_CYPHER_T mode, const u8* key, 
    u8* input_output_buffer, const size_t buffer_length)
{
    assert(buffer_length % 16 == 0);

    u8 round_keys[4 * 4 * 15];
    u8 state[4 * 4];

    expand_key(mode, key, round_keys);

    for (size_t i = 0; i < buffer_length / 16; i++)
    {
        // add round key
        for (size_t j = 0; j < 4 * 4; j++)
        {
            // Assume the length of input is a multiple of 16
            state[j] = input_output_buffer[16 * i + j] ^ round_keys[4 * number_of_rounds[mode] + j];
        }

        inv_shift_rows(state);
        inv_sub_bytes(state);

        for (int round = number_of_rounds[mode] - 1; round >= 1; round--)
        {
            add_round_key(state, round_keys, round);
            inv_mix_columns(state);
            inv_shift_rows(state);
            inv_sub_bytes(state);
        }

        for (size_t j = 0; j < 4 * 4; j++)
        {
            // Assume the length of input is a multiple of 16
            input_output_buffer[16 * i + j] = state[j] ^ round_keys[j];
        }
    }
}