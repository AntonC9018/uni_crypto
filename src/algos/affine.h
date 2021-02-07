#pragma once
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <vector>

#include <mydefines.h>
#include <strlib.h>
#include "../shared.h"

namespace Affine
{
    struct Key
    {
        char a;
        char b;
        char a_inv;
    };

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

    inline Key make_key(char a, char b)
    {
        if (!are_coprime(a, LATIN_LENGTH))
        {
            report_error("a (%i) must be coprime to the total character count (%i).\n", a, LATIN_LENGTH);
        }
        
        Key key;
        key.a = a;
        key.b = b;
        key.a_inv = (char)inverse_modulo(a, LATIN_LENGTH);

        return key;
    }

    void encrypt_and_decrypt_messages(const std::vector<str_view_t>& messages, char a, char b)
    {
        auto key = make_key(a, b);

        for (auto m : messages)
        {
            auto encrypted = encrypt(m, key);
            auto decrypted = decrypt(str_view(encrypted), key);
            printf("%s -> %s -> %s\n", m.chars, encrypted.chars, decrypted.chars);
            str_free(encrypted); 
            str_free(decrypted);
        }
    }
}
