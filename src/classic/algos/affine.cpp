#include "affine.h"

namespace Affine
{
    str_t encrypt(str_view_t message, Key key)
    {
        str_t encrypted_message = str_make(message.length);
        for (size_t i = 0; i < message.length; i++)
        {
            char mod = ((message[i] - FIRST_CHARACTER) * key.a + key.b) % LATIN_LENGTH;
            encrypted_message[i] = mod + FIRST_CHARACTER;
        }
        return encrypted_message;
    }

    str_t decrypt(str_view_t encrypted_message, Key key)
    {
        str_t decrypted_message = str_make(encrypted_message.length);
        for (size_t i = 0; i < encrypted_message.length; i++)
        {
            char mod = ((decrypted_message[i] - FIRST_CHARACTER - key.b + LATIN_LENGTH) * key.a_inv) % LATIN_LENGTH;
            decrypted_message[i] = mod + FIRST_CHARACTER;
        }
        return decrypted_message;
    }
}
