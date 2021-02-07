#pragma once
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <vector>

#include <mydefines.h>
#include "../shared.h"
#include "common.h"

namespace Vigenere
{
    struct Key
    {
        str_view_t keyword;
    };

    // We assume only plain lower case latin letters are used
    static str_t xxcrypt(str_view_t message, const Key& key, Crypto_Action action)
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

    inline str_t encrypt(str_view_t message, const Key& key)
    {
        return xxcrypt(message, key, ENCRYPT);
    }

    inline str_t decrypt(str_view_t encrypted, const Key& key)
    {
        return xxcrypt(encrypted, key, DECRYPT);
    }
}