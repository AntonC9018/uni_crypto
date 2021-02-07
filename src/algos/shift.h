#pragma once
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <vector>

#include <mydefines.h>
#include "../shared.h"

namespace Shift
{
    struct Key
    {
        std::vector<size_t> row_perm;
        std::vector<size_t> col_perm;
    };

    inline void destroy_key(Key& key)
    {
        key.row_perm.clear();
        key.col_perm.clear();
    }

    // If a letter starts off at position x, y, it will end up at
    // lin_perm[x], col_perm[y]. To go back, find the encrypted x and y
    // in these lists and take the index.
    // I'm going to represent the letters as a linear array.
    str_t encrypt(str_view_t message, const Key& key)
    {
        // The dimensions of the output message are going to be width * height of the key
        str_t encrypted = str_make(key.col_perm.size() * key.row_perm.size());
        // Fill the buffer with spaces at start.
        memset(encrypted.chars, ' ', encrypted.length);

        // Make sure the length of the message is no more than that.
        if (message.length > encrypted.length)
        {
            report_error("The message is too large. Max size for the given key is %uz, but the message had size %uz. Here it is: %s", 
                encrypted.length, message.length, message.chars);
        }

        // Fill in the spots with the characters permuted according to the key.
        size_t index = 0;
        for (size_t row = 0; row < key.row_perm.size(); row++)
        {
            for (size_t col = 0; col < key.col_perm.size(); col++, index++)
            {
                if (message[index] == 0)
                    return encrypted;
                
                encrypted[key.row_perm[row] * key.col_perm.size() + key.col_perm[col]] = message[index];
            }
        }

        return encrypted;
    }

    str_t decrypt(str_view_t encrypted, const Key& key)
    {
        // The dimensions of the output message are going to be width * height of the key
        str_t decrypted = str_make(key.col_perm.size() * key.row_perm.size());

        // May as well validate the encrypted message
        if (encrypted.length != decrypted.length)
        {
            report_error("Unexpected encrypted message length. Expected %zu, got %zu", 
                decrypted.length, decrypted.length); 
        }

        size_t index = 0;
        for (size_t row = 0; row < key.row_perm.size(); row++)
        {
            for (size_t col = 0; col < key.col_perm.size(); col++, index++)
            {
                // Find the row and the column indices in the key
                size_t new_row = find_index(key.row_perm, row);
                size_t new_col = find_index(key.col_perm, col);
                decrypted[new_row * key.col_perm.size() + new_col] = encrypted[index];
            }
        }
        return decrypted;
    }
}