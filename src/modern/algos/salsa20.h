#include <mydefines.h>

// This function expects a little endian system
void salsa20_crypt16(
    u8 key_16_byte[16], u8 nonce[8], 
    u8 *input_output_buffer, u32 buffer_length);

// This function expects a little endian system
void salsa20_crypt32(
    u8 key_32_byte[32], u8 nonce[8], 
    u8 *input_output_buffer, u32 buffer_length);