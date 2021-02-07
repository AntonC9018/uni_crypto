#include "straddling.h"

namespace Straddling
{
    Key make_key(const Make_Params& mp)
    {
        Key key;
        {
            size_t i = 0;
            while (mp.keyword[i] != 0)
            {
                if (mp.code_positions[i] >= mp.dim)
                {
                    report_error("Height limit of %i exceeded", mp.dim);
                }
                key.encrypt_header[mp.keyword[i]] = { mp.code_positions[i] };
                key.decrypt_header[{ mp.code_positions[i] }] = mp.keyword[i];
                i++;
            }
        }

        size_t i = 0;
        size_t j = 0;
        while (in_map(key.decrypt_header, mp.order[i]))
        {
            i++; 
            error_if_over_limit_normal(mp, i);
        }
        auto current_char_ptr = mp.char_set.chars;
        while (true)
        {
            key.encrypt_normal[*current_char_ptr] = { mp.order[i], mp.order[j] };
            key.decrypt_normal[{ mp.order[i], mp.order[j] }] = *current_char_ptr;
            current_char_ptr++;
            j++;

            if (*current_char_ptr == 0)
            {
                break;
            }

            if (j == mp.dim)
            {
                j = 0;
                do
                {
                    i++; 
                    error_if_over_limit_normal(mp, i); 
                    error_if_over_limit_normal(mp, j);
                } 
                while (in_map(key.decrypt_header, mp.order[i]));
            }
        }

        return key;
    };

    Key make_key(str_view_t keyword, const std::vector<size_t>& indices, str_view_t scramble)
    {
        Make_Params mp;
        mp.keyword = keyword;                                   // not owned
        mp.char_set = alphabet_without_keyword(keyword);        // owned
        mp.order = arrange(scramble, latin_numbers_underscore); // owned
        mp.code_positions = without(mp.order, indices);         // owned
        mp.dim = 10;

        Key key = make_key(mp);
        destroy_params(&mp);
        return key;
    }

    void destroy_key(Key& key)
    {
        key.encrypt_header.clear();
        key.encrypt_normal.clear();
        key.decrypt_header.clear();
        key.decrypt_normal.clear();
    }
    
    void print_key(Key key)
    {
        char table[12][11];
        memset(table, '-', 12 * 11);
        for (char i = 0; i < 10; i++)
        {
            table[0][i + 1] = '0' + i;
        }
        for (auto [key, value] : key.encrypt_header)
        {
            table[1][value + 1] = key;
        }
        for (auto [key, value] : key.encrypt_normal)
        {
            table[2 + value.first][1 + value.second] = key;
            table[2 + value.first][0] = value.first + '0';
        }
        for (size_t i = 0; i < 12; i++)
        {
            for (size_t j = 0; j < 11; j++)
            {
                printf("%c ", table[i][j]); 
            }
            printf("\n");
        }
    }

    std::vector<size_t> encrypt(str_view_t message, const Key& key)
    {
        std::vector<size_t> encrypted_message;
        for (int i = 0; i < message.length; i++)
        {
            if (in_map(key.encrypt_header, message[i]))
            {
                encrypted_message.push_back(key.encrypt_header.at(message[i]));
            }
            else if (in_map(key.encrypt_normal, message[i]))
            {
                auto val = key.encrypt_normal.at(message[i]);
                encrypted_message.push_back(val.first);
                encrypted_message.push_back(val.second);
            }
            else
            {
                report_error("The character %c is not present in the dictionary.", message[i]);
            }
        }
        return std::move(encrypted_message);
    }

    str_t decrypt(const std::vector<size_t>& encrypted_message, const Key& key)
    {
        str_builder_t decrypted = strb_make(encrypted_message.size());
        size_t i = 0;

        while (i < encrypted_message.size())
        {
            size_t index;
            if (in_map(key.decrypt_header, encrypted_message[i]))
            {
                strb_chr(decrypted, key.decrypt_header.at(encrypted_message[i]));
            }
            else 
            {
                Normal_Encrypted_Value encrypted_key = { encrypted_message[i], encrypted_message[i + 1] };   
                if (in_map(key.decrypt_normal, encrypted_key))
                {
                    strb_chr(decrypted, key.decrypt_normal.at(encrypted_key));
                    i++;
                }
                else
                {
                    report_error("The encrypted combination (%i, %i) is not present in the dictionary.",
                        encrypted_key.first, encrypted_key.second);
                    return STR_NULL;
                }
            }
            i++;
        }
        return strb_build(decrypted);
    }
}