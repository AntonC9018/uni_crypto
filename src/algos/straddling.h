#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <vector>
#include <map>

#include "shared.h"


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

    // template<int CodeLength, int Dim>
    // struct Make_Params
    // {
    //     char keyword[CodeLength];
    //     int code_position[CodeLength];
    //     int order[Dim];
    //     char char_set[(Dim - CodeLength) * Dim];
    // }; 

    // TODO: clean memory of vectors.
    struct Make_Params
    {
        const char* keyword; // not owned
        std::vector<size_t> order;
        std::vector<size_t> code_positions;
        const char* char_set; // owned
        size_t dim;
    };

    void destroy_params(Make_Params* mp)
    {
        str_free(mp->char_set);
        mp->order.clear();
        mp->code_positions.clear();
    }

    inline void error_if_over_limit_normal(Make_Params mp, size_t i)
    {
        if (i >= mp.dim || mp.order[i] >= mp.dim)
        {
            report_error("Height limit of %zu exceeded. Character: %c, Position: %zu", mp.dim, *mp.char_set, i);
        }
    }

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
        auto current_char_ptr = mp.char_set;
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

    Key make_key(const char* keyword, const std::vector<size_t>& indices, const char* scramble)
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

    std::vector<size_t> encrypt(const char* message, const Key& key)
    {
        std::vector<size_t> encrypted_message;
        while (*message != 0)
        {
            if (in_map(key.encrypt_header, *message))
            {
                encrypted_message.push_back(key.encrypt_header.at(*message));
            }
            else if (in_map(key.encrypt_normal, *message))
            {
                auto val = key.encrypt_normal.at(*message);
                encrypted_message.push_back(val.first);
                encrypted_message.push_back(val.second);
            }
            else
            {
                report_error("The character %c is not present in the dictionary.", *message);
            }
            message++;
        }
        return std::move(encrypted_message);
    }

    const char* decrypt(const std::vector<size_t>& encrypted_message, const Key& key)
    {
        char* decrypted_message = (char*) malloc(encrypted_message.size());
        char* current = decrypted_message;
        size_t i = 0;
        while (i < encrypted_message.size())
        {
            size_t index;
            if (in_map(key.decrypt_header, encrypted_message[i]))
            {
                *current = key.decrypt_header.at(encrypted_message[i]);
            }
            else 
            {
                Normal_Encrypted_Value encrypted_key = { encrypted_message[i], encrypted_message[i + 1] };   
                if (in_map(key.decrypt_normal, encrypted_key))
                {
                    *current = key.decrypt_normal.at(encrypted_key);
                    i++;
                }
                else
                {
                    report_error("The encrypted combination (%i, %i) is not present in the dictionary.",
                        encrypted_key.first, encrypted_key.second);
                }
            }
            i++;
            current++;
        }
        *current = 0;
        return decrypted_message;
    }
}