#include "bazeries.h"

namespace Bazeries
{
    str_t take_alphabet_through_map_and_transpose(size_t dim, str_view_t alphabet, Letter_Map mapf)
    {
        str_t str = Abc::take_alphabet_through_map(alphabet, mapf);
        
        if (str.length != dim * dim)
        {
            report_error("Unexpected alphabet size %zu. Expected %zu", str.length, dim * dim);
        }

        // All that's left is to transpose the chars
        str_t alpha = str_make(str.length);

        for (size_t row = 0; row < dim; row++)
        {
            for (size_t col = 0; col < dim; col++)
            {
                alpha[col * dim + row] = str[row * dim + col];   
            }
        }

        str_free(str);

        return alpha;
    }

    str_t xxcrypt(str_view_t message, const Key& key, Crypto_Action action)
    {
        str_t result = str_make(message.length);

        size_t current_block_index = 0;
        size_t message_index       = 0;
        size_t group_offset        = 0;

        while (group_offset < message.length)
        {
            size_t current_block_length = std::min(
                (size_t)key.numeric_keyword[current_block_index], message.length - group_offset);
            size_t in_block_index = current_block_length - 1;

            while ((s32)in_block_index >= 0)
            {
                size_t result_index = group_offset + in_block_index;

                if (action == ENCRYPT)
                {
                    size_t ch_index = str_find_char_index(str_view(key.transposed_alphabet), message[message_index]);
                    if (ch_index == (size_t)-1)
                    {
                        report_error("Character %c not found in string %s", 
                            message[message_index], key.transposed_alphabet.chars);
                    }
                    result[result_index] = key.poly.table[ch_index];
                }
                else
                {
                    size_t ch_index = str_find_char_index(str_view(key.poly.table), message[message_index]);
                    if (ch_index == (size_t)-1)
                    {
                        report_error("Character %c not found in string %s", 
                            message[message_index], key.poly.table.chars);
                    }
                    result[result_index] = key.transposed_alphabet[ch_index];
                }

                in_block_index--;
                message_index++;
            }
            group_offset += current_block_length;
            current_block_index = (current_block_index + 1) % key.numeric_keyword.size();
        }

        return result;
    }

    void destroy_key(Key& key)
    {
        Polybios::destroy_key(key.poly);
        str_free(key.transposed_alphabet);
        key.numeric_keyword.clear();
        std::vector<char>().swap(key.numeric_keyword);
    }
    
    str_t delimit(str_view_t plain_text, const Key& key)
    {
        auto delimited_sb = strb_make(plain_text.length * 2);

        size_t current_count = 0;
        size_t numeric_keyword_index = 0;

        for (size_t i = 0; i < plain_text.length; i++)
        {
            if (current_count == (size_t)key.numeric_keyword[numeric_keyword_index])
            {
                strb_chr(delimited_sb, ' ');
                numeric_keyword_index++;
                if (numeric_keyword_index == key.numeric_keyword.size())
                {
                    numeric_keyword_index = 0;
                }
                current_count = 0;
            }

            char ch = plain_text[i];
            strb_chr(delimited_sb, ch);
            current_count++;
        }
        return strb_build(delimited_sb);
    }

}