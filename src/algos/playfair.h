#pragma once
#include <map>
#include <vector>
#include "polybios.h"
#include "../shared.h"
#include "../abc.h"
#include "common.h"

namespace Playfair
{
    using namespace Abc;

    using Polybios::Key;
    using Polybios::make_key;
    using Polybios::print_key;

    struct Coord
    {
        size_t row; 
        size_t col;
    };

    inline Coord find_coord(char letter, Key key)
    {
        size_t i = find(letter, key);
        return { i / key.dim, i % key.dim }; 
    }

    // Displaces up or down
    inline Coord wrap_shift_row(Coord coord, Key key, int dir)
    {
        return { (coord.row + key.dim + dir) % key.dim, coord.col };
    }

    // Displaces to the right or to the left
    inline Coord wrap_shift_col(Coord coord, Key key, int dir)
    {
        return { coord.row, (coord.col + key.dim + dir) % key.dim };
    }

    inline Coord wrap_shift_right(Coord coord, Key key)
    {
        return { (coord.row + 1) % key.dim, coord.col };
    }

    inline Coord wrap_shift_down(Coord coord, Key key)
    {
        return { coord.row, (coord.col + 1) % key.dim };
    }

    inline Coord wrap_shift_left(Coord coord, Key key)
    {
        return { (coord.row + key.dim - 1) % key.dim, coord.col };
    }

    inline Coord wrap_shift_up(Coord coord, Key key)
    {
        return { coord.row, (coord.col + key.dim - 1) % key.dim };
    }

    inline char look_up(Coord coord, Key key)
    {
        return key.table[coord.row * key.dim + coord.col];
    }


    typedef std::pair<char, char> Digram;

    inline void subst(std::vector<Digram>& diagrams, char subst_char = 'X')
    {
        for (auto& d : diagrams)
        {
            if (d.second == d.first)
            {
                d.second = subst_char;
            }
        }
    }

    inline void unsubst(std::vector<Digram>& diagrams, char subst_char = 'X')
    {
        for (auto& d : diagrams)
        {
            if (d.second == subst_char)
            {
                d.second = d.first;
            }
        }
    }

    std::vector<Digram> make_encryption_digrams(const char* message, const Key& key, char subst_char = 'X')
    {
        size_t i = 0;
        std::vector<Digram> result;

        while (message[i] != 0)
        {
            if (message[i + 1] == 0)
            {
                result.push_back({ key.mapf(message[i]), subst_char });
                break;
            }
            result.push_back({ key.mapf(message[i]), key.mapf(message[i + 1]) });

            i += 2;
        }

        return std::move(result);
    }

    // Assumes the length of message is even
    // Also assumes the used character sets match
    std::vector<Digram> make_decryption_digrams(const char* message)
    {
        size_t i = 0;
        std::vector<Digram> result;
        while (message[i] != 0)
        {
            result.push_back({ message[i], message[i + 1] });
            i += 2;
        }
        return std::move(result);
    }

    // If dir == 1,  the rules for encryption are applied 
    // If dir == -1, the rules for decryption are applied
    void apply_rules(std::vector<Digram>& digrams, const Key& key, Crypto_Action action)
    {
        for (auto& digram : digrams)
        {
            printf("%c%c -> ", digram.first, digram.second);

            auto f = find_coord(digram.first, key);
            auto s = find_coord(digram.second, key);
            
            // 2. If the letters appear on the same row of your table, replace them 
            //    with the letters to their immediate right respectively.
            if (f.row == s.row)
            {
                digram.first = look_up(wrap_shift_col(f, key, action), key);
                digram.second = look_up(wrap_shift_col(s, key, action), key);
                printf("%c%c (row)\n", digram.first, digram.second);
                continue;
            }

            // 3. If the letters appear on the same column of your table, replace them 
            //    with the letters immediately below respectively.
            if (f.col == s.col)
            {
                digram.first = look_up(wrap_shift_row(f, key, action), key);
                digram.second = look_up(wrap_shift_row(s, key, action), key);
                printf("%c%c (col)\n", digram.first, digram.second);
                continue;
            }

            // Otherwise take the opposite sides of the rectangle
            digram.first = look_up({ f.row, s.col }, key);
            digram.second = look_up({ s.row, f.col }, key);
            printf("%c%c (rect)\n", digram.first, digram.second);
        }
    }

    const char* join_digrams(const std::vector<Digram>& digrams)
    {
        char* buffer = (char*) malloc(digrams.size() * 2 + 1);
        printf("Joining digrams: ");
        for (size_t i = 0; i < digrams.size(); i++)
        {
            printf("%c%c ", digrams[i].first, digrams[i].second);
            buffer[2 * i] = digrams[i].first;
            buffer[2 * i + 1] = digrams[i].second;
        }
        printf("\n");
        buffer[digrams.size() * 2] = 0;
        return buffer;
    }

    void encrypt_digrams(std::vector<Digram>& digrams, const Key& key, char subst_char = 'X')
    {
        subst(digrams, subst_char);
        apply_rules(digrams, key, ENCRYPT);
    }

    const char* encrypt(const char* message, const Key& key, char subst_char = 'X')
    {
        auto digrams = make_encryption_digrams(message, key, subst_char);
        encrypt_digrams(digrams, key, subst_char);
        return join_digrams(digrams);
    }

    void decrypt_digrams(std::vector<Digram>& digrams, const Key& key, char subst_char = 'X')
    {
        apply_rules(digrams, key, DECRYPT);
        unsubst(digrams, subst_char);
    }

    const char* decrypt(const char* message, const Key& key, char subst_char = 'X')
    {
        auto digrams = make_decryption_digrams(message);
        decrypt_digrams(digrams, key, subst_char);
        return join_digrams(digrams);
    }
}