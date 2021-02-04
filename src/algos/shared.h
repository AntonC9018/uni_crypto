#pragma once
#include <vector>
#include <algorithm>

#define FIRST_CHARACTER 'a'
#define LAST_CHARACTER 'z'
#define LATIN_LENGTH (LAST_CHARACTER - FIRST_CHARACTER + 1)

#define in_map(map, key) map.find((key)) != map.end()

const char* latin = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
const char* latin_numbers = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
const char* latin_numbers_underscore = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_";

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
const char* validate(const char* m, const char* alphabet = latin)
{
    while(*m != 0)
    {
        if (strchr(alphabet, *m) == 0)
        {
            return m;
        }
        m++;
    }
    return 0;
}

const char* alphabet_without_keyword(
    const char* keyword, const char* alphabet = latin_numbers_underscore)
{
    char* result = (char*) malloc(strlen(alphabet) + 1);
    size_t i = 0;
    while (*alphabet != 0)
    {
        if (strchr(keyword, *alphabet) == 0)
        {
            result[i] = *alphabet;
            i++;
        }
        *alphabet++;                
    }
    result[i] = '\0';
    return result;
}

inline bool contains(const std::vector<size_t>& v, size_t item)
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

struct Thing { size_t index; size_t value; };

std::vector<size_t> arrange(const char* string, const char* alphabet = latin_numbers_underscore)
{
    std::vector<size_t> result;
    std::vector<Thing> things;
    while (*string != 0)
    {
        size_t pos = (size_t)strchr(alphabet, *string);
        if (!contains(result, pos))
        {
            result.push_back(pos);
            things.push_back({ things.size(), pos });
        }
        string++;
    }
    sort(things.begin(), things.end(), [](auto a, auto b) { return a.value < b.value; });
    for (size_t i = 0; i < result.size(); i++)
    {
        result[i] = things[i].index;
    }
    return std::move(result);
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
    return std::move(result);
}

const char* leave_unique(const char* but, const char* all)
{
    // TODO: validate the first parameter (all characters from it should be contained within the second)
    char* result = (char*) malloc(strlen(all) + 1);
    size_t i = 0;
    while (but[i] != 0)
    {
        result[i] = but[i];
        i++;
    }
    while (*all != 0)
    {
        if (strchr(but, *all) == 0)
        {
            result[i] = *all;
            i++;
        }
        all++;
    }
    result[i] = 0;
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