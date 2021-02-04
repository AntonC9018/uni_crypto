#pragma once
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <vector>

#include <mydefines.h>
#include "shared.h"

namespace Vigenere
{
    struct Key
    {
        const char* keyword;
    };

    // We assume only plain lower case latin letters are used
    const char* encrypt(const char* message, const Key& key)
    {
        size_t message_length = strlen(message);
        char* result = (char*) malloc(message_length + 1);
        result[message_length] = 0;
        
        const char* keyword = key.keyword;

        for (size_t i = 0; i < message_length; i++)
        {
            result[i] = ((*keyword - FIRST_CHARACTER) + (message[i] - FIRST_CHARACTER)) 
                % LATIN_LENGTH + FIRST_CHARACTER;
            keyword++;
            if (*keyword == 0)
            {
                keyword = key.keyword;
            }
        }

        return result;
    }

    const char* decrypt(const char* encrypted, const Key& key)
    {
        size_t message_length = strlen(encrypted);
        char* result = (char*) malloc(message_length + 1);
        result[message_length] = 0;

        const char* keyword = key.keyword;

        for (size_t i = 0; i < message_length; i++)
        {
            result[i] = ((encrypted[i] - FIRST_CHARACTER) - (*keyword - FIRST_CHARACTER) + LATIN_LENGTH)
                % LATIN_LENGTH + FIRST_CHARACTER;
            keyword++;
            if (*keyword == 0)
            {
                keyword = key.keyword;
            }
        }

        return result;
    }
}