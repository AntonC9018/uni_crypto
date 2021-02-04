#pragma once
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <vector>

#include <mydefines.h>
#include "shared.h"

namespace Affine
{
    struct Key
    {
        char a;
        char b;
        char a_inv;
    };

    const char* encrypt(const char* message, Key key)
    {
        size_t len = strlen(message);
        char* encrypted_message = (char*) malloc(len + 1);
        for (size_t i = 0; i < len; i++)
        {
            char mod = ((message[i] - FIRST_CHARACTER) * key.a + key.b) % num_chars;
            encrypted_message[i] = mod + FIRST_CHARACTER;
        }
        encrypted_message[len] = 0;
        return encrypted_message;
    }

    const char* decrypt(const char* encrypted_message, Key key)
    {
        size_t len = strlen(encrypted_message);
        char* decrypted_message = (char*) malloc(len + 1);
        for (size_t i = 0; i < len; i++)
        {
            char mod = ((encrypted_message[i] - FIRST_CHARACTER - key.b + num_chars) * key.a_inv) % num_chars;
            decrypted_message[i] = mod + FIRST_CHARACTER;
        }
        decrypted_message[len] = 0;
        return decrypted_message;
    }

    inline Key make_key(char a, char b)
    {
        if (!are_coprime(a, num_chars))
        {
            report_error("a (%i) must be coprime to the total character count (%i).\n", a, num_chars);
        }
        
        Key key;
        key.a = a;
        key.b = b;
        key.a_inv = (char)inverse_modulo(a, num_chars);

        return key;
    }

    void encrypt_and_decrypt_messages(const std::vector<const char*>& messages, char a, char b)
    {
        auto key = make_key(a, b);

        for (auto m : messages)
        {
            auto encrypted = encrypt(m, key);
            auto decrypted = decrypt(encrypted, key);
            printf("%s -> %s -> %s\n", m, encrypted, decrypted);
            free(encrypted); 
            free(decrypted);
        }
    }
}
