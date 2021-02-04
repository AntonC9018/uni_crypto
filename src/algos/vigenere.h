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
        const char* keyword;
    };

    // We assume only plain lower case latin letters are used
    internal const char* xxcrypt(const char* message, const Key& key, Crypto_Action action)
    {
        size_t message_length = strlen(message);
        char* result = (char*) malloc(message_length + 1);
        result[message_length] = 0;

        const char* keyword = key.keyword;

        for (size_t i = 0; i < message_length; i++)
        {
            result[i] = ((message[i] - FIRST_CHARACTER) + action * (*keyword - FIRST_CHARACTER) + LATIN_LENGTH)
                % LATIN_LENGTH + FIRST_CHARACTER;
            keyword++;
            if (*keyword == 0)
            {
                keyword = key.keyword;
            }
        }

        return result;
    }

    inline const char* encrypt(const char* message, const Key& key)
    {
        return xxcrypt(message, key, ENCRYPT);
    }

    inline const char* decrypt(const char* encrypted, const Key& key)
    {
        return xxcrypt(encrypted, key, DECRYPT);
    }
}