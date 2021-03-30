#include <mydefines.h>

enum AES_CYPHER_T {
    AES_CYPHER_128,
    AES_CYPHER_192,
    AES_CYPHER_256,
};

static const int rijndael_key_length_in_dwords[] = {
    /* AES_CYPHER_128 */  4,
    /* AES_CYPHER_192 */  6,
    /* AES_CYPHER_256 */  8,
};

void rijndael_encrypt(AES_CYPHER_T mode, const u8* key, 
    u8* input_output_buffer, const size_t buffer_length);

void rijndael_decrypt(AES_CYPHER_T mode, const u8* key, 
    u8* input_output_buffer, const size_t buffer_length);