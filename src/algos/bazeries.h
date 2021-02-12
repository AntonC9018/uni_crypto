#pragma once
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <algorithm>
#include <vector>

#include <mydefines.h>
#include "../shared.h"
#include "polybios.h"
#include "../abc.h"
#include "common.h"


namespace Bazeries
{
    using namespace Abc;

    struct Key 
    {
        Polybios::Key poly;
        std::vector<char> numeric_keyword;
        str_t transposed_alphabet;
    };

    str_t take_alphabet_through_map_and_transpose(size_t dim, str_view_t alphabet, Letter_Map mapf);

    inline Key make_key(str_view_t keyword, const std::vector<char>& numeric_keyword, 
        size_t dim = 5, Letter_Map mapf = map_letter_default, str_view_t alphabet = latin)
    {
        Key key;
        key.poly = Polybios::make_key(dim, keyword, mapf, alphabet);
        key.transposed_alphabet = take_alphabet_through_map_and_transpose(dim, alphabet, mapf);
        key.numeric_keyword = numeric_keyword;
        return key;
    }

    void destroy_key(Key& key);

    str_t xxcrypt(str_view_t message, const Key& key, Crypto_Action action);

    inline str_t encrypt(str_view_t message, const Key& key)
    {
        return xxcrypt(message, key, ENCRYPT);
    }

    inline str_t decrypt(str_view_t encrypted, const Key& key)
    {
        return xxcrypt(encrypted, key, DECRYPT);
    }    

    str_t delimit(str_view_t plain_text, const Key& key);
}