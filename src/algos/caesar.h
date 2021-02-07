#pragma once
#include <string.h>
#include <stdlib.h>
#include "../shared.h"

namespace Caesar
{
    // Decrypt the message into the specified buffer
    void decrypt(char* dest, str_view_t encrypted_message, str_view_t decryption_key)
    {
        char c;
        size_t i = 0;
        while ((c = encrypted_message[i]) != 0)
        {
            dest[i] = decryption_key[c - FIRST_CHARACTER];
            i++;
        }
    }

    str_t decrypt(str_view_t encrypted_message, str_view_t decryption_key)
    {
        str_t result = str_make(encrypted_message.length);
        decrypt(result.chars, encrypted_message, decryption_key);
        return result;
    }

    // Encrypt the message into the specified buffer
    void encrypt(char* dest, str_view_t message, str_view_t encryption_key)
    {
        char c;
        size_t i = 0;
        while ((c = message.chars[i]) != 0)
        {
            dest[i] = encryption_key[c - FIRST_CHARACTER];
            i++;
        }
        dest[i] = 0;
    }

    str_t encrypt(str_view_t message, str_view_t encryption_key)
    {
        str_t result = str_make(message.length);
        encrypt(result.chars, message, encryption_key);
        return result;
    }

    str_t make_encryption_key(str_view_t keyword)
    {
        // We'll be filling up this buffer
        str_t encryption_key = str_make(LATIN_LENGTH);
        // We'll be putting characters into this index
        size_t current_index = 0;

        // Construct the 'met' table, which contains the characters already in the generated key
        // It will be useful for filling in the rest of the characters in alphabetical order
        bool met[LATIN_LENGTH] = {0};

        // Insert the keyword first
        for (size_t i = 0; i < keyword.length; i++)
        {
            char c = keyword[i];
            if (!met[c - FIRST_CHARACTER])
            {
                encryption_key[current_index] = c;
                current_index++;
            }
            met[c - FIRST_CHARACTER] = true;            
        }

        // Fill in the rest of the alphabet
        for (char c = FIRST_CHARACTER; c <= LAST_CHARACTER; c++)
        {
            // Skip the characters that are already in the key
            if (met[c - FIRST_CHARACTER])
            {
                continue;
            }
            encryption_key[current_index] = c;
            current_index++;
        }

        return encryption_key;
    }

    str_t make_decryption_key(str_view_t encryption_key)
    {
        str_t decryption_key = str_make(LATIN_LENGTH);
        for (char i = 0; i < decryption_key.length; i++)
        {
            char index = encryption_key[i] - FIRST_CHARACTER;
            decryption_key[index] = i + FIRST_CHARACTER;
        }
        return decryption_key;
    }
}