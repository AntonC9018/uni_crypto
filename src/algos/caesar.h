#pragma once
#include <string.h>
#include <stdlib.h>
#include "shared.h"

namespace Caesar
{
    // Decrypt the message into the specified buffer
    void decrypt(char* dest, const char* encrypted_message, const char* decryption_key)
    {
        char c;
        size_t i = 0;
        while ((c = encrypted_message[i]) != 0)
        {
            dest[i] = decryption_key[c - FIRST_CHARACTER];
            i++;
        }
        dest[i] = 0;
    }

    const char* decrypt(const char* encrypted_message, const char* decryption_key)
    {
        char* buffer = (char*) malloc(strlen(encrypted_message) + 1);
        decrypt(buffer, encrypted_message, decryption_key);
        return buffer;
    }

    // Encrypt the message into the specified buffer
    void encrypt(char* dest, const char* message, const char* encryption_key)
    {
        char c;
        size_t i = 0;
        while ((c = message[i]) != 0)
        {
            dest[i] = encryption_key[c - FIRST_CHARACTER];
            i++;
        }
        dest[i] = 0;
    }

    const char* encrypt(const char* message, const char* encryption_key)
    {
        char* buffer = (char*) malloc(strlen(message) + 1);
        encrypt(buffer, message, encryption_key);
        return buffer;
    }

    const char* make_encryption_key(const char* keyword)
    {
        // We'll be filling up this buffer
        char* encryption_key = (char*)malloc(LATIN_LENGTH);
        // We'll be putting characters into this index
        size_t current_index = 0;

        // Construct the 'met' table, which contains the characters already in the generated key
        // It will be useful for filling in the rest of the characters in alphabetical order
        char c;
        bool met[LATIN_LENGTH] = {0};

        // Insert the keyword first
        while ((c = *keyword) != 0)
        {
            if (!met[c - FIRST_CHARACTER])
            {
                encryption_key[current_index] = c;
                current_index++;
            }
            met[c - FIRST_CHARACTER] = true;            
            keyword++;
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

    const char* make_decryption_key(const char* encryption_key)
    {
        char* decryption_key = (char*)malloc(LATIN_LENGTH);
        for (char i = 0; i < LATIN_LENGTH; i++)
        {
            char index = encryption_key[i] - FIRST_CHARACTER;
            decryption_key[index] = i + FIRST_CHARACTER;
        }
        return decryption_key;
    }
}