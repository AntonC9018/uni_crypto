#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <vector>

// Don't use this. See playwright.h. 
// In its most basic form, It is equivalent to Straddling.
namespace Straddling
{
    #define num_columns 10

    struct Header
    {
        char elements[num_columns];

        bool contains(char c) const
        {
            for (char el : elements)
            {
                if (c == el) 
                    return true;
            }
            return false;
        }
    };

    struct Row
    {
        int index;
        char elements[num_columns];
    };
    
    struct Key
    {
        Header header;
        std::vector<Row> rows;

        void print()
        {
            // Print header numbers
            printf("  ");
            for (int i = 0; i <= 9; i++)
            {
                // :For Playwright indices remap before printing
                printf("%i ", i);
            }
            printf("\n  ");

            // Print header characters
            for (char e : header.elements)
            {
                printf("%c ", e == 0 ? '-' : e);
            }
            printf("\n");

            // Print rows
            for (const Row& row : rows)
            {
                // Row number
                printf("%i ", row.index);
                // Row character
                for (char e : row.elements)
                {
                    printf("%c ", e == 0 ? '-' : e);
                }
                printf("\n");
            }
        }
    };

    void make_key(Key* key, const char* codeword, const std::vector<int>& indices_to_use, const char* char_set)
    {
        memset(key->header.elements, 0, num_columns);
        for (int i : indices_to_use)
        {
            key->header.elements[i] = *codeword;
            codeword++;
        }

        for (int i = 0; i < num_columns; i++)
        {
            if (key->header.elements[i] == 0)
            {
                // Add a row
                key->rows.emplace_back();
                Row& row = key->rows.back();
                row.index = i;

                int j = 0;
                while (j < num_columns)
                {
                    if (*char_set == 0)
                    {
                        memset(&row.elements[j], 0, num_columns - j);
                        break;
                    }
                    row.elements[j] = *char_set;
                    j++; char_set++;
                }
            }
        }
    }
}