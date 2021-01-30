#pragma once
#define FIRST_CHARACTER 'a'
#define LAST_CHARACTER 'z'
#define num_chars (LAST_CHARACTER - FIRST_CHARACTER + 1)

#define in_map(map, key) map.find((key)) != map.end()

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