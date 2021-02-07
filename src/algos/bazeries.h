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

    static str_t take_alphabet_through_map_and_transpose(size_t dim, str_view_t alphabet, Letter_Map mapf)
    {
        str_t str = Abc::take_alphabet_through_map(alphabet, mapf);
        
        if (str.length != dim * dim)
        {
            report_error("Unexpected alphabet size %zu. Expected %zu", str.length, dim * dim);
        }

        // All that's left is to transpose the chars
        str_t alpha = str_make(str.length);

        for (size_t row = 0; row < dim; row++)
        {
            for (size_t col = 0; col < dim; col++)
            {
                alpha[col * dim + row] = str[row * dim + col];   
            }
        }

        str_free(str);

        return alpha;
    }

    inline Key make_key(str_view_t keyword, std::vector<char> numeric_keyword, 
        size_t dim = 5, Letter_Map mapf = map_letter_default, str_view_t alphabet = latin)
    {
        Key key;
        key.poly = Polybios::make_key(dim, keyword, mapf, alphabet);
        key.transposed_alphabet = take_alphabet_through_map_and_transpose(dim, alphabet, mapf);
        key.numeric_keyword = numeric_keyword;
        return key;
    }

    inline void destroy_key(Key& key)
    {
        Polybios::destroy_key(key.poly);
        str_free(key.transposed_alphabet);
        key.numeric_keyword.clear();
    }

    static str_t xxcrypt(str_view_t message, const Key& key, Crypto_Action action)
    {
        str_t result = str_make(message.length);

        size_t current_block_index = 0;
        size_t i = 0;
        size_t j = 0;

        while (i < message.length)
        {
            size_t current_block_length = std::min((size_t)key.numeric_keyword[current_block_index], message.length - i);
            size_t in_block_index = current_block_length - 1;

            while ((s32)in_block_index >= 0)
            {
                size_t index = i + in_block_index;

                if (action == ENCRYPT)
                {
                    size_t ch_index = str_find_char_index(str_view(key.transposed_alphabet), message[j]);
                    if (ch_index == -1)
                    {
                        report_error("Character %c not found in string %s", message[j], key.transposed_alphabet.chars);
                    }
                    result[index] = key.poly.table[ch_index];
                }
                else
                {
                    size_t ch_index = str_find_char_index(str_view(key.poly.table), message[j]);
                    if (ch_index == -1)
                    {
                        report_error("Character %c not found in string %s", message[j], key.poly.table.chars);
                    }
                    result[index] = key.transposed_alphabet[ch_index];
                }

                in_block_index--;
                j++;
            }
            i += current_block_length;
            current_block_index = (current_block_index + 1) % key.numeric_keyword.size();
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