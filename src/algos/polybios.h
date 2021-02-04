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
        const char* table;
        Letter_Map mapf;
    };

    inline void destroy_key(Key& key)
    {
        str_free(key.table);
    }

    Key make_key(size_t dim, const char* keyword, 
        Letter_Map mapf = map_letter_default, const char* alphabet = latin)
    {
        size_t table_size = dim * dim;
        char* table = (char*) malloc(table_size);
        memset(table, 0, table_size);

        size_t i = 0;

        // First, put in the keyword
        while (*keyword != 0)
        {
            if (strchr(table, mapf(*keyword)) == 0)
            {
                table[i] = *keyword;
                i++;
            }
            keyword++;
        }

        // Second, the rest of the alphabet
        while (i < table_size) 
        {
            if (*alphabet == 0)
            {
                report_error("Not enough characters in the alphabet to fill up the key table.");
            }
            if (strchr(table, mapf(*alphabet)) == 0)
            {
                table[i] = *alphabet;
                i++;
            }
            alphabet++;
        }

        return { dim, table, mapf };
    }

    inline size_t find(char letter, Key key)
    {
        return find_index(key.table, letter, key.dim * key.dim);
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