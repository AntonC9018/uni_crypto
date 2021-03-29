#pragma once
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <vector>

#include <mydefines.h>
#include <shared/util.h>
#include "common.h"

namespace Vigenere
{
    struct Key
    {
        str_view_t keyword;
    };

    // We assume only plain lower case latin letters are used
    str_t xxcrypt(str_view_t message, const Key& key, Crypto_Action action);

    inline str_t encrypt(str_view_t message, const Key& key)
    {
        return xxcrypt(message, key, ENCRYPT);
    }

    inline str_t decrypt(str_view_t encrypted, const Key& key)
    {
        return xxcrypt(encrypted, key, DECRYPT);
    }
}