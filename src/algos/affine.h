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

    str_t encrypt(str_view_t message, Key key);

    str_t decrypt(str_view_t encrypted_message, Key key);

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

    void encrypt_and_decrypt_messages(const std::vector<str_view_t>& messages, char a, char b);
}
