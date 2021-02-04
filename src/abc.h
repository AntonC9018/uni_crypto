#pragma once

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
}