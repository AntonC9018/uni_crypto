#include "abc.h"

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
    str_t take_alphabet_through_map(str_view_t alphabet, Letter_Map mapf)
    {
        str_builder_t a = strb_make(alphabet.length);

        for (size_t j = 0; j < alphabet.length; j++)
        {
            char ch = mapf(alphabet[j]);
            if (!strb_has_char(a, ch))
            {
                strb_chr(a, ch);
            }
        }

        return strb_build(a);
    }
}