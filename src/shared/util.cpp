#include "util.h"

str_view_t latin = str_lit("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
str_view_t latin_numbers = str_lit("ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");
str_view_t latin_numbers_underscore = str_lit("ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_");

// Removes all occurences of a character after it has been met
void prune_string(char* dest, const char* src)
{
    char c;
    bool met[LATIN_LENGTH] = {0};
    while ((c = *src) != 0)
    {
        if (!met[c - FIRST_CHARACTER])
        {
            *dest = c;
            dest++;
        }
        met[c - FIRST_CHARACTER] = true;            
        src++;
    }
    *dest = 0;
}

int gcd(int x, int y)
{
    while (x != y)
    {
        (x > y) ? (x -= y) : (y -= x);
    }
    return x;
}

bool are_coprime(int x, int y)
{
    return gcd(x, y) == 1;
}

// Returns modulo inverse of a with respect to m using extended Euclid Algorithm
// Assumption: a and m are coprime
int inverse_modulo(int a, int m0)
{
    int m = m0;
    int y = 0;
    int x = 1;
 
    if (m == 1)
        return 0;
 
    while (a > 1) {
        int q = a / m;
        int t = m;
 
        // m is remainder now, process same as Euclid's algo
        m = a % m;
        a = t;
        t = y;
 
        // Update y and x
        y = x - q * y;
        x = t;
    }
 
    // Make x positive
    if (x < 0)
        x += m0;
 
    return x;
}

// This function validates the message/keyword against the given alphabet.
// Returns a pointer to the problematic character.
const char* validate(str_view_t m, str_view_t alphabet)
{
    for (size_t i = 0; i < m.length; i++)
    {
        if (!str_has_char(alphabet, m[i]))
        {
            return &m.chars[i];
        }
    }
    return 0;
}

str_t alphabet_without_keyword(str_view_t keyword, str_view_t alphabet)
{
    str_builder_t result = strb_make(alphabet.length);

    for (size_t j = 0; j < alphabet.length; j++)
    {
        if (!str_has_char(keyword, alphabet[j]))
        {
            strb_chr(result, alphabet[j]);
        }
    }
    return strb_build(result);
}

bool contains(const std::vector<size_t>& v, size_t item)
{
    for (auto i : v)
    {
        if (i == item)
        {
            return true;
        }
    }
    return false;
}

std::vector<size_t> arrange(str_view_t string, str_view_t alphabet)
{
    std::vector<_Arrange_Temporary> things;
    std::vector<size_t> result;

    for (size_t i = 0; i < string.length; i++)
    {
        size_t char_index = str_find_char_index(alphabet, string[i]);
        if (!contains(result, char_index))
        {
            result.push_back(char_index);
            things.push_back({ things.size(), char_index });
        }
    }
    sort(things.begin(), things.end(), [](auto a, auto b) { return a.value < b.value; });

    for (size_t i = 0; i < things.size(); i++)
    {
        result[i] = things[i].index;
    }
    
    return result;
}

std::vector<size_t> without(const std::vector<size_t>& order, const std::vector<size_t>& but)
{
    std::vector<size_t> result;
    result.reserve(order.size() - but.size());
    for (size_t i : order)
    {
        if (!contains(but, i))
        {
            result.push_back(i);
        }
    }
    // Due to copy elision, this seems to be better than
    // result std::move(result);
    // Since no copy is actually going to be created.
    return result;
}

str_t leave_unique(str_view_t but, str_view_t all)
{
    // TODO: validate the first parameter (all characters from it should be contained within the second)
    str_t result = str_make(all.length);
    memcpy(result.chars, but.chars, but.length);

    size_t i = but.length;
    for (size_t j = 0; j < all.length; j++)
    {
        if (!str_has_char(but, all[j]))
        {
            result[i] = all[j];
            i++;
        }
    }
    return result;
}

size_t find_index(const std::vector<size_t>& v, size_t item)
{
    for (size_t i = 0; i < v.size(); i++)
    {
        if (v[i] == item)
        {
            return i;
        }
    }
    return -1;
}