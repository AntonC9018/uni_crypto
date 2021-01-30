#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <vector>
#include <map>

#include "shared.h"



namespace Playwright
{
    const char* default_alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_";

    const char* alphabet_without_codeword(
        const char* codeword, const char* alphabet = default_alphabet)
    {
        char* result = (char*)malloc(sizeof(default_alphabet));
        int i = 0;
        while (*alphabet != 0)
        {
            if (strchr(codeword, *alphabet) == 0)
            {
                result[i] = *alphabet;
                i++;
            }
            *alphabet++;                
        }
        result[i] = '\0';
        return result;
    }

    typedef int Header_Encrypted_Value;
    typedef std::pair<int, int> Normal_Encrypted_Value;

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
    //     char codeword[CodeLength];
    //     int code_position[CodeLength];
    //     int order[Dim];
    //     char char_set[(Dim - CodeLength) * Dim];
    // }; 

    // TODO: clean memory of vectors.
    struct Make_Params
    {
        const char* codeword;
        std::vector<int> order;
        std::vector<int> code_positions;
        const char* char_set;
        int dim;
    };

    inline void error_if_over_limit_normal(Make_Params mp, int i)
    {
        if (i >= mp.dim || mp.order[i] >= mp.dim)
        {
            fprintf(stderr, "Height limit of %i exceeded. Character: %c, Position: %i", mp.dim, *mp.char_set, i);
            exit(-1);
        }
    }

    Key make_key(Make_Params mp)
    {
        Key key;
        
        {
            int i = 0;
            while (mp.codeword[i] != 0)
            {
                if (mp.code_positions[i] >= mp.dim)
                {
                    fprintf(stderr, "Height limit of %i exceeded", mp.dim);
                    exit(-1);
                }
                key.encrypt_header[mp.codeword[i]] = { mp.code_positions[i] };
                key.decrypt_header[{ mp.code_positions[i] }] = mp.codeword[i];
                i++;
            }
        }

        int i = 0;
        int j = 0;
        while (in_map(key.decrypt_header, mp.order[i]))
        {
            i++; 
            error_if_over_limit_normal(mp, i);
        }
        while (true)
        {
            key.encrypt_normal[*mp.char_set] = { mp.order[i], mp.order[j] };
            key.decrypt_normal[{ mp.order[i], mp.order[j] }] = *mp.char_set;
            mp.char_set++;
            j++;

            if (*mp.char_set == 0)
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

    // This is equivalent to Straddling
    Key make_key_default(const char* codeword)
    {
        Make_Params mp;
        mp.codeword = codeword;
        mp.char_set = alphabet_without_codeword(codeword);
        mp.dim = 10;
        mp.order = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

        int codeword_length = strlen(codeword);
        mp.code_positions.reserve(codeword_length);
        for (int i = 0; i < codeword_length; i++)
        {
            mp.code_positions.push_back(i);
        }

        return make_key(mp);
    }

    void destroy_key(Key key)
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
        for (int i = 0; i < 10; i++)
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
        for (int i = 0; i < 12; i++)
        {
            for (int j = 0; j < 11; j++)
            {
                printf("%c ", table[i][j]); 
            }
            printf("\n");
        }
    }

    std::vector<int> encrypt(const char* message, Key key)
    {
        std::vector<int> encrypted_message;
        while (*message != 0)
        {
            if (key.encrypt_header.find(*message) != key.encrypt_header.end())
            {
                encrypted_message.push_back(key.encrypt_header[*message]);
            }
            else if (key.encrypt_normal.find(*message) != key.encrypt_normal.end())
            {
                auto val = key.encrypt_normal[*message];
                encrypted_message.push_back(val.first);
                encrypted_message.push_back(val.second);
            }
            else
            {
                fprintf(stderr, "The character %c is not present in the dictionary.", *message);
                exit(-1);
            }
            message++;
        }
        return std::move(encrypted_message);
    }

    const char* decrypt(const std::vector<int>& encrypted_message, Key key)
    {
        char* decrypted_message = (char*)malloc(encrypted_message.size());
        char* current = decrypted_message;
        int i = 0;
        while (i < encrypted_message.size())
        {
            int index;
            if (key.decrypt_header.find(encrypted_message[i]) != key.decrypt_header.end())
            {
                *current = key.decrypt_header[encrypted_message[i]];
            }
            else 
            {
                Normal_Encrypted_Value encrypted_key = { encrypted_message[i], encrypted_message[i + 1] };   
                if (key.decrypt_normal.find(encrypted_key) != key.decrypt_normal.end())
                {
                    *current = key.decrypt_normal[encrypted_key];
                    i++;
                }
                else
                {
                    fprintf(stderr, "The encrypted combination (%i, %i) is not present in the dictionary.",
                        encrypted_key.first, encrypted_key.second);
                    exit(-1);
                }
            }
            i++;
            current++;
        }
        *current = 0;
        return decrypted_message;
    }
}