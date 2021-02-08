#pragma once
#include <vector>
#include <algorithm>
#include <strlib.h>

#define FIRST_CHARACTER 'a'
#define LAST_CHARACTER 'z'
#define LATIN_LENGTH (LAST_CHARACTER - FIRST_CHARACTER + 1)

#define in_map(map, key) map.find((key)) != map.end()

extern str_view_t latin;
extern str_view_t latin_numbers;
extern str_view_t latin_numbers_underscore;

inline bool is_digit(char ch)
{
    return ch >= '0' && ch <= '9';
}

// Removes all occurences of a character after it has been met
void prune_string(char* dest, const char* src);

int gcd(int x, int y);

bool are_coprime(int x, int y);

// Returns modulo inverse of a with respect to m using extended Euclid Algorithm
// Assumption: a and m are coprime
int inverse_modulo(int a, int m0);

// This function validates the message/keyword against the given alphabet.
// Returns a pointer to the problematic character.
const char* validate(str_view_t m, str_view_t alphabet = latin);

str_t alphabet_without_keyword(str_view_t keyword, 
                               str_view_t alphabet = latin_numbers_underscore);

bool contains(const std::vector<size_t>& v, size_t item);

struct _Arrange_Temporary { size_t index; size_t value; };

std::vector<size_t> arrange(str_view_t string, str_view_t alphabet = latin_numbers_underscore);

std::vector<size_t> without(const std::vector<size_t>& order, const std::vector<size_t>& but);

str_t leave_unique(str_view_t but, str_view_t all);

size_t find_index(const std::vector<size_t>& v, size_t item);