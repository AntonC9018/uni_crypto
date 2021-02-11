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
        Letter_Map mapf = map_letter_default, str_view_t alphabet = latin);

    inline size_t find(char letter, Key key)
    {
        return str_find_char_index(str_view(key.table), letter);
    }

    void print_key(Key key);
}