#pragma once
#include <map>
#include <vector>

#include <mydefines.h>
#include "../shared.h"
#include "../abc.h"


namespace Polybios
{
    using namespace Abc;
    
    struct Key
    {
        size_t dim;
        str_t table;
        Letter_Map mapf;
    };

    inline void destroy_key(Key& key)
    {
        str_free(key.table);
    }

    Key make_key(size_t dim, str_view_t keyword, 
        Letter_Map mapf = map_letter_default, str_view_t alphabet = latin)
    {
        // Make sure the entire alphabet will fit
        if (alphabet.length < dim * dim)
        {
            report_error("Not enough characters in the alphabet to fill up the key table.");
        }

        Key key;
        key.dim = dim;
        key.mapf = mapf;
        key.table = str_make_zeros(dim * dim);

        size_t i = 0;

        // First, put in the keyword
        for (size_t j = 0; j < keyword.length; j++)
        {
            if (!str_has_char(key.table, mapf(keyword[j])))
            {
                key.table[i] = keyword[j];
                i++;
            }
        }

        // Second, the rest of the alphabet
        for (size_t j = 0; j < alphabet.length; j++) 
        {
            if (!str_has_char(key.table, mapf(alphabet[j])))
            {
                if (i == key.table.length)
                {
                    report_error("Too many characters in the key table");
                }
                key.table[i] = alphabet[j];
                i++;
            }
        }

        return key;
    }

    inline size_t find(char letter, Key key)
    {
        return str_find_char_index(str_view(key.table), letter);
    }

    void print_key(Key key)
    {
        printf("  ");
        for (size_t j = 0; j < key.dim; j++)
        {
            printf("%zu ", j);
        }
        printf("\n");
        for (size_t i = 0; i < key.dim; i++)
        {
            printf("%zu ", i);
            for (size_t j = 0; j < key.dim; j++)
            {
                printf("%c ", key.table[i * key.dim + j]);
            }
            printf("\n");
        }
    }
}