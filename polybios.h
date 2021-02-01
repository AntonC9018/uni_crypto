#pragma once
#include "shared.h"
#include <map>
#include <vector>
#include "abc.h"

namespace Polybios
{
    using namespace Abc;
    
    struct Key
    {
        int dim;
        const char* table;
        Letter_Map mapf;
    };

    Key make_key(int dim, const char* keyword, 
        Letter_Map mapf = map_letter_default, const char* alphabet = latin)
    {
        int table_size = dim * dim;
        char* table = (char*) malloc(table_size);
        memset(table, 0, table_size);

        int i = 0;

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
                fprintf(stderr, "Not enough characters in the alphabet to fill up the key table.");
                exit(-1);
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

    int find(char letter, Key key)
    {
        for (int i = 0; i < key.dim * key.dim; i++)
        {
            if (key.table[i] == letter)
                return i;
        }
        // if we're here, the letter is not in the table
        return -1;
    }

    void print_key(Key key)
    {
        printf("  ");
        for (int j = 0; j < key.dim; j++)
        {
            printf("%i ", j);
        }
        printf("\n");
        for (int i = 0; i < key.dim; i++)
        {
            printf("%i ", i);
            for (int j = 0; j < key.dim; j++)
            {
                printf("%c ", key.table[i * key.dim + j]);
            }
            printf("\n");
        }
    }
}