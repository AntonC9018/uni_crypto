#pragma once
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <vector>

#include <mydefines.h>
#include <shared/util.h>

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

    str_t encrypt(str_view_t message, const Key& key);
    str_t decrypt(str_view_t encrypted, const Key& key);
    size_t max_message_size(const Key& key);
}