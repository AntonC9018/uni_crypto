#include "vigenere.h"

namespace Vigenere
{
    // We assume only plain upper case latin letters are used
    str_t xxcrypt(str_view_t message, const Key& key, Crypto_Action action)
    {
        str_t result = str_make(message.length);
        size_t j = 0;

        for (size_t i = 0; i < message.length; i++)
        {
            result[i] = ((message[i] - FIRST_CHARACTER) + action * (key.keyword[j] - FIRST_CHARACTER) + LATIN_LENGTH) % LATIN_LENGTH + FIRST_CHARACTER;

            j++;
            if (j == key.keyword.length) 
            {
                j = 0;
            }
        }

        return result;
    }
}