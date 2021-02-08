#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <mydefines.h>

#include <vector>
#include <map>

#include "../shared.h"


namespace Straddling
{
    typedef size_t Header_Encrypted_Value;
    typedef std::pair<size_t, size_t> Normal_Encrypted_Value;

    struct Key
    {
        // hash maps for header and the rows
        std::map<char, Header_Encrypted_Value> encrypt_header;
        std::map<char, Normal_Encrypted_Value> encrypt_normal;
        // and mirrored decrypt pairs
        std::map<Header_Encrypted_Value, char> decrypt_header;
        std::map<Normal_Encrypted_Value, char> decrypt_normal;
    };

    // struct Make_Params
    // {
    //     str_view_t keyword; // not owned
    //     std::vector<size_t> order;
    //     std::vector<size_t> row_indices;
    //     str_t char_set; // owned
    //     size_t dim;
    // };

    // inline void destroy_params(Make_Params* mp)
    // {
    //     str_free(mp->char_set);
    //     mp->order.clear();
    //     mp->row_indices.clear();
    // }

    // inline void error_if_over_limit_normal(Make_Params mp, size_t i)
    // {
    //     if (i >= mp.dim || mp.order[i] >= mp.dim)
    //     {
    //         report_error("Height limit of %zu exceeded. Character: %c, Position: %zu", mp.dim, mp.char_set[i], i);
    //     }
    // }

    Key make_key(str_view_t keyword,
        const std::vector<size_t>& order,
        const std::vector<size_t>& row_indices,
        str_view_t char_set,
        size_t dim
    );

    Key make_key(str_view_t keyword, const std::vector<size_t>& indices, str_view_t scramble);

    void destroy_key(Key& key);

    void print_key(Key key);

    std::vector<size_t> encrypt(str_view_t message, const Key& key);

    str_t decrypt(const std::vector<size_t>& encrypted_message, const Key& key);
}