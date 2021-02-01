#pragma once
#include <vector>
#include <algorithm>

#define FIRST_CHARACTER 'a'
#define LAST_CHARACTER 'z'
#define num_chars (LAST_CHARACTER - FIRST_CHARACTER + 1)

#define in_map(map, key) map.find((key)) != map.end()

const char* latin = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
const char* latin_numbers = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
const char* latin_numbers_underscore = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_";

// Removes all occurences of a character after it has been met
void prune_string(char* dest, const char* src)
{
    char c;
    bool met[num_chars] = {0};
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
    char* result = (char*) malloc(sizeof(latin_numbers_underscore));
    int i = 0;
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

inline bool contains(const std::vector<int> v, int item)
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

struct Thing { int index; int value; };

std::vector<int> arrange(const char* string, const char* alphabet = latin_numbers_underscore)
{
    std::vector<int> result;
    std::vector<Thing> things;
    while (*string != 0)
    {
        int pos = (int)strchr(alphabet, *string) ;
        if (!contains(result, pos))
        {
            result.push_back(pos);
            things.push_back({ (s32)things.size(), pos });
        }
        string++;
    }
    sort(things.begin(), things.end(), [](auto a, auto b) { return a.value < b.value; });
    for (int i = 0; i < result.size(); i++)
    {
        result[i] = things[i].index;
    }
    return std::move(result);
}

std::vector<int> without(const std::vector<int>& order, const std::vector<int>& but)
{
    std::vector<int> result;
    result.reserve(order.size() - but.size());
    for (int i : order)
    {
        if (!contains(but, i))
        {
            result.push_back(i);
        }
    }
    return std::move(result);
}

const char* join_unique(const char* first, const char* all)
{
    // TODO: validate the first parameter (all characters from it should be contained within the second)
    char* result = (char*) malloc(strlen(all) + 1);
    int i = 0;
    while (first[i] != 0)
    {
        result[i] = first[i];
        i++;
    }
    while (*all != 0)
    {
        if (strchr(first, *all) == 0)
        {
            result[i] = *all;
            i++;
        }
        all++;
    }
    result[i] = 0;
    return result;
}