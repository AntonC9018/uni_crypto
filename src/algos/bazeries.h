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
        const char* transposed_alphabet;
    };

    internal char* take_alphabet_through_map_and_transpose(size_t dim, const char* alphabet, Letter_Map mapf)
    {
        char* chars = Abc::take_alphabet_through_map(dim * dim, alphabet, mapf);
        // All that's left is to transpose the chars
        char* alpha = (char*) malloc(dim * dim);
        for (size_t row = 0; row < dim; row++)
        {
            for (size_t col = 0; col < dim; col++)
            {
                alpha[col * dim + row] = chars[row * dim + col];   
            }
        }

        return alpha;
    }

    inline Key make_key(const char* keyword, std::vector<char> numeric_keyword, 
        size_t dim = 5, Letter_Map mapf = map_letter_default, const char* alphabet = latin)
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

    internal const char* xxcrypt(const char* message, const Key& key, Crypto_Action action)
    {
        size_t message_length = strlen(message);
        char* result = (char*) malloc(message_length + 1);
        result[message_length] = 0;

        size_t current_block_index = 0;
        size_t i = 0;

        while (i < message_length)
        {
            size_t current_block_length = std::min((size_t)key.numeric_keyword[current_block_index], message_length - i);
            size_t in_block_index = current_block_length - 1;

            while ((s32)in_block_index >= 0)
            {
                size_t index = i + in_block_index;

                if (action == ENCRYPT)
                {
                    char ch_index = find_index(key.transposed_alphabet, *message, key.poly.dim * key.poly.dim);
                    char ch = key.poly.table[ch_index];
                    result[index] = ch;
                }
                else
                {
                    char ch_index = find_index(key.poly.table, *message, key.poly.dim * key.poly.dim);
                    char ch = key.transposed_alphabet[ch_index];
                    result[index] = ch;
                }

                in_block_index--;
                message++;
            }
            i += current_block_length;
            current_block_index = (current_block_index + 1) % key.numeric_keyword.size();
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