#include "straddling.h"

namespace Straddling
{
    Key make_key(
        str_view_t keyword,
        const std::vector<size_t>& order,
        const std::vector<size_t>& row_indices,
        str_view_t char_set,
        size_t dim
    )
    {
        auto code_positions = without(order, row_indices);
        Key key;

        for (size_t i = 0; i < code_positions.size(); i++)
        {
            if (code_positions[i] >= dim)
            {
                report_error("Height limit of %i exceeded", dim);
            }
            key.encrypt_header[keyword[i]] = { code_positions[i] };
            key.decrypt_header[{ code_positions[i] }] = keyword[i];
        }

        size_t col = 0;
        size_t index = 0;

        for (size_t row : row_indices)
        {
            for (size_t col = 0; col < dim; col++)
            {
                if (char_set[index] == 0)
                {
                    report_error("Yikes, the char_set is not valid");
                }

                key.encrypt_normal[char_set[index]] = { row, order[col] };
                key.decrypt_normal[{ row, order[col] }] = char_set[index];
                index++;
            }
        }

        return key;
    };

    Key make_key(str_view_t keyword, const std::vector<size_t>& indices, str_view_t scramble)
    {
        auto char_set = alphabet_without_keyword(keyword);
        Key key = make_key(
            keyword, 
            arrange(scramble, latin_numbers_underscore), 
            indices, 
            str_view(char_set), 
            10
        );
        str_free(char_set);
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