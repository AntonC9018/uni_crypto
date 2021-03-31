#include "trivium.h"
#include "endianness.h"
#include <string.h>
#include <assert.h>

inline u8 bit_at(u8 _byte, u32 position)
{
    return (_byte >> position) & 1;
}

inline u8 bit(u8 s[36], u32 position)
{
    u32 byte_index = position / 8;
    u8 bit_index   = position % 8;
    return bit_at(s[byte_index], bit_index);
}

inline void set_bit(u8 s[36], u32 position, u8 bit)
{
    u32 byte_index = position / 8;
    u8 bit_index   = position % 8;
    u8 byte_cleared_at_position = s[byte_index] & ((1 << bit_index) ^ 0xFF);
    s[byte_index] = byte_cleared_at_position | (bit << bit_index);
}

// Rotate bytes by an amount less than 8
inline void rotate_bytes_right(u8* _bytes, u32 num_bytes, u8 by_amount)
{
    assert(by_amount < 8);
    u8 previous_carry_over = 0;

    for (u32 i = 0; i < num_bytes; i++)
    {
        u8 carry_over = _bytes[i] >> (8 - by_amount);
        _bytes[i] = (_bytes[i] << by_amount) | previous_carry_over;
        previous_carry_over = carry_over;
    }
}

inline void rotate_state_right_by_one(u8 s[36])
{
    rotate_bytes_right(s, 36, 1);
}

static void initialize(u8 s[36], u8 key[10], u8 ivector[10])
{
    memcpy(&s[0], key, 10);
    s[10] = 0; // reset bits 80 through 88

    {
        u8 ivector_shifted_temporary[11];
        memcpy(ivector_shifted_temporary, ivector, 10);
        ivector_shifted_temporary[10] = 0;

        // Now, shift it to the right (in little endian sense) by 5
        // We will copy the result into the state at position 88, but the
        // initialization vector must end up starting at 93.
        rotate_bytes_right(ivector_shifted_temporary, 11, 5);

        // set bits up to 176
        memcpy(&s[11], ivector_shifted_temporary, 11);
    }

    // reset the rest of memory but the last byte to zeros
    memset(&s[22], 0, 13);
    // set the last 3 bits to 1
    s[35] = 0xE0;

    for (int i = 0; i < 4 * 288; i++)
    {
        u8 t1 = bit(s, 65)  ^ (bit(s, 90)  & bit(s, 91))  ^ bit(s, 92)  ^ bit(s, 170);
        u8 t2 = bit(s, 161) ^ (bit(s, 174) & bit(s, 175)) ^ bit(s, 176) ^ bit(s, 263);
        u8 t3 = bit(s, 242) ^ (bit(s, 285) & bit(s, 286)) ^ bit(s, 287) ^ bit(s, 68);

        rotate_state_right_by_one(s);
        set_bit(s, 0, t3); set_bit(s, 93, t1); set_bit(s, 177, t2);
    }
}

void trivium_crypt(u8 key[10], u8 ivector[10], u8* input_output_buffer, size_t length)
{
    u8 s[36];
    initialize(s, key, ivector);

    for (size_t i = 0; i < length; i++)
    {
        for (u8 j = 0; j < 8; j++)
        {
            u8 t1 = bit(s, 65)  ^ bit(s, 92);
            u8 t2 = bit(s, 161) ^ bit(s, 176);
            u8 t3 = bit(s, 242) ^ bit(s, 287);
            u8 key_bit = t1 ^ t2 ^ t3;

            u8 key_bit_in_correct_position = key_bit << j;
            input_output_buffer[i] = input_output_buffer[i] ^ key_bit_in_correct_position;

            t1 = t1 ^ (bit(s, 90)  & bit(s, 91))  ^ bit(s, 170);
            t2 = t2 ^ (bit(s, 174) & bit(s, 175)) ^ bit(s, 263);
            t3 = t3 ^ (bit(s, 285) & bit(s, 286)) ^ bit(s, 68);

            rotate_state_right_by_one(s);
            set_bit(s, 0, t3); set_bit(s, 93, t1); set_bit(s, 177, t2);
        }
    }
}