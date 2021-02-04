#pragma once
#include "shared.h"

namespace Abc
{
    typedef char(*Letter_Map)(char); 
    // or std::function<char, char>
    // or use templates + one general functor (this makes most sense to me, actually).

    // Maps J to I and leaves the rest the same
    char map_letter_default(char letter)
    {
        if (letter == 'J') return 'I';
        return letter;
    }

    // Maps any letter to itself. Might be useful if you make sure that your message 
    // does not use any of the characters not in the table.
    char map_letter_identity(char letter)
    {
        return letter;
    }

    // Takes the given alphabet through the given map
    char* take_alphabet_through_map(size_t expected_size, const char* alphabet, Letter_Map mapf)
    {
        char* a = (char*) calloc(1, expected_size);
        for (size_t i = 0; i < expected_size; i++)
        {
            while (strchr(a, mapf(*alphabet)) != 0 && *alphabet != 0)
                alphabet++;
            
            if (*alphabet == 0)
            {
                report_error("Wrong number of characters for this size");
                return NULL;
            }

            a[i] = *alphabet;
            alphabet++;
        }
        return a;
    }
}