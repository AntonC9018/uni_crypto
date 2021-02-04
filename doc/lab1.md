## Laborator 1.

### 1.
> Pentru a aplica un sistem de criptare, utilizatorul introduce cheia sub forma unei parole din 8 litere mici ale alfabetului latin, unde fiecare literă este codificată,  
> a) folosind schema ASCII (7 biți per caracter).  
> b) folosind numere din intervalul 00-25.  
> Care este lungimea în biți a cheii corespunzătoare?  

a) Orice caracter din ASCII este de obicei pastrat intr-un singur byte. Aceasta simplifica citirea lor, si ofera compatibilitatea cu UTF-8. In asa caz lungimea ar fi egala cu numarul de caractere in parola (8). Insa se poate pastra textul in mod *packed*, unde fiecare caracter de fapt ocupa 8 biti (fara acel padding de un bit), deci lungimea ar fi (8\*7)/8 = 7 bytes sau (8\*7) = 56 biti.

b) 26 caractere se pot incape in 5 biti (2^5 = 32).

### 2.
> Pentru a aplica un sistem de criptare, utilizatorul introduce cheia din r
> a) caractere ASCII (8 biți per caracter);  
> b) litere mici ale alfabetului latin.  
> Lungimea în biți a cheii generate este 128 biți. Aflați r.  

a) r = 128 / 8 = 16 caractere.

b) Literele latine pot fi reprezentate prin 26 de caractere = 5 biti pentru fiecare caracter. 128/5 = 25.6. Deci, codificarea nu a putut sa fie pe 5 biti. Numarul de biti minim care divizeaza 128 si care este mai mare decat 5 este 8, deci b) este echivalenta cu a) (16 caractere).

### 3.
> Textul clar are 2000 de simboluri. Acesta va fi criptat cu utilizarea unui cifru bloc pe 64 biți. Dacă pentru codificare este utilizat ASCII pe 8 biți, determinați numărul de biți necesari pentru completarea ultimului bloc și numărul total de blocuri ale mesajului.

2000*8/64 = 2000/8 = 250 blocuri.
Fiecare bloc este codificat utilizand 64 biti (2000 este divizibil cu 8).
(Cifru bloc = permutari?).

### 4.
> O rețea de utilizatori online are 100 de membri. Răspundeți la următoarele întrebări:  
> a) Câte chei secrete sunt necesare dacă toți membrii rețelei vor să transmită mesaje secrete între ei, folosind criptografia cu cheie simetrică.  
> b) Câte chei secrete sunt necesare dacă fiecare membru al rețelei are încredere în administratorul acesteia? Dacă unmembru al rețelei vrea să transmită un mesaj altui membru, acesta transmite, mai întâi, mesajul către administrator (care este unul dintre membri), iar administratorul transmite mesajul către membrul destinatar.  
> c) Oricare doi membri ai rețelei, care solicită o sesiune de comunicare, urmează, mai întâi, să contacteze administratorul rețelei. Administratorul generează o cheie cu funcționalitate temporară, care va fi utilizată în cadrul sesiunii de comunicare între acești doi utilizatori. Cheia temporară este criptată și apoi transmisă ambilor utilizatori. Câte chei secrete sunt necesare pentru a asigura comunicarea între oricare doi utilizatori ai rețelei?

a) Daca presupunem ca fiecare pereche de utilizatori trebuie sa aiba o cheie unica, atunci raspunsul este 99\*100/2 = 4950.

b) Cate o cheie pentru fiecare pereche utilizator - administrator (100).

c) Cate o cheie permanenta pentru fiecare pereche utilizator - administrator (100) + cate o cheie pentru fiecare pereche de utilizatori posibile, daca presupunem ca fiecare pereche de utilizatori trebuie sa aiba (potential simultan) o cheie unica (4950). Deci, 5050.


## Laborator 2

### Cezar din doc. (1)

Folosind sistemul de criptare Caesar cu cheie, decriptați textul criptat "rkilusop", dacă cheia secretă este "martor".

Am scris un program pentru aceasta. Am pus codul pentru (de)criptare intr-un fisier `.h` (oricum il voi utiliza numai intr-un singur fisier).

```c++
// caesar.h
#pragma once
#include <string.h>
#include <stdlib.h>
#include "shared.h"

namespace Caesar
{
    // Decrypt the message into the specified buffer
    void decrypt(char* dest, const char* encrypted_message, const char* decryption_key)
    {
        char c;
        int i = 0;
        while ((c = encrypted_message[i]) != 0)
        {
            dest[i] = decryption_key[c - FIRST_CHARACTER];
            i++;
        }
        dest[i] = 0;
    }

    // Encrypt the message into the specified buffer
    void encrypt(char* dest, const char* message, const char* encryption_key)
    {
        char c;
        int i = 0;
        while ((c = message[i]) != 0)
        {
            dest[i] = encryption_key[c - FIRST_CHARACTER];
            i++;
        }
        dest[i] = 0;
    }

    const char* make_encryption_key(const char* keyword)
    {
        // We'll be filling up this buffer
        char* encryption_key = (char*)malloc(num_chars);
        // We'll be putting characters into this index
        int current_index = 0;
        // Construct the 'met' table, which contains the characters already in the generated key
        // It will be useful for filling in the rest of the characters in alphabetical order
        char c;
        bool met[num_chars] = {0};
        while ((c = *keyword) != 0)
        {
            if (!met[c - FIRST_CHARACTER])
            {
                encryption_key[current_index] = c;
                current_index++;
            }
            met[c - FIRST_CHARACTER] = true;            
            keyword++;
        }
        // Fill in the rest of the alphabet.
        for (char c = FIRST_CHARACTER; c <= LAST_CHARACTER; c++)
        {
            if (met[c - FIRST_CHARACTER])
            {
                continue;
            }
            encryption_key[current_index] = c;
            current_index++;
        }

        return encryption_key;
    }

    const char* make_decryption_key(const char* encryption_key)
    {
        char* decryption_key = (char*)malloc(num_chars);
        for (char i = 0; i < num_chars; i++)
        {
            char index = encryption_key[i] - FIRST_CHARACTER;
            decryption_key[index] = i + FIRST_CHARACTER;
        }
        return decryption_key;
    }
}
```

Shared defineste unele constante.

```c++
// shared.h
#define FIRST_CHARACTER 'a'
#define LAST_CHARACTER 'z'
#define num_chars (LAST_CHARACTER - FIRST_CHARACTER + 1)
```

Si utilizarea codului dat din main.cpp.

```c++
// main.cpp
#include <stdio.h>
#include "caesar.h"

int main()
{
    const char* encrypted = "rkilusop";
    const char* keyword = "martor";

    const char* encryption_key = Caesar::make_encryption_key(keyword);
    const char* decryption_key = Caesar::make_decryption_key(encryption_key);
    
    char decrypted_message[sizeof(encrypted)];
    Caesar::decrypt(decrypted_message, encrypted, decryption_key);
    puts(decrypted_message);
}
```

Codul dat decripteaza mesajul "rkilusop" in "computer".

### 1. GUI

Voi face un GUI utilizand gtkmm. (In progress).

### 2. Folosind cifrul afin, criptați textul clar.

> a) "the tree is a good hiding place"  
> b) "hide the gold in the tree stump"  
> În funcția de criptare se vor considera următoarele valori pentru coeficienții ași b:  
> a) 7, 5; b) 5, 8.
> Decriptați mesajul obținut.

```c++
// affine.h
#pragma once
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <vector>

#include <mydefines.h> // contine definitii pentru s64, s32, u64, u32 etc.
#include "shared.h"

namespace Affine
{
    struct Key
    {
        char a;
        char b;
        char a_inv;
    };

    const char* encrypt(const char* message, Key key)
    {
        s64 len = strlen(message);
        char* encrypted_message = (char*)malloc(len + 1);
        for (s64 i = 0; i < len; i++)
        {
            char mod = ((message[i] - FIRST_CHARACTER) * key.a + key.b) % num_chars;
            encrypted_message[i] = mod + FIRST_CHARACTER;
        }
        encrypted_message[len] = 0;
        return encrypted_message;
    }

    const char* decrypt(const char* encrypted_message, Key key)
    {
        s64 len = strlen(encrypted_message);
        char* decrypted_message = (char*)malloc(len + 1);
        for (s64 i = 0; i < len; i++)
        {
            char mod = ((encrypted_message[i] - FIRST_CHARACTER - key.b + num_chars) * key.a_inv) % num_chars;
            decrypted_message[i] = mod + FIRST_CHARACTER;
        }
        decrypted_message[len] = 0;
        return decrypted_message;
    }

    inline Key make_key(char a, char b)
    {
        if (!are_coprime(a, num_chars))
        {
            fprintf(stderr, "a (%i) must be coprime to the total character count (%i).\n", a, num_chars);
            exit(-1);
        }
        
        Key key;
        key.a = a;
        key.b = b;
        key.a_inv = (char)inverse_modulo(a, num_chars);

        return key;
    }

    void encrypt_and_decrypt_messages(const std::vector<const char*>& messages, char a, char b)
    {
        auto key = make_key(a, b);

        for (auto m : messages)
        {
            auto encrypted = encrypt(m, key);
            auto decrypted = decrypt(encrypted, key);
            printf("%s -> %s -> %s\n", m, encrypted, decrypted);
            free(encrypted); 
            free(decrypted);
        }
    }
}
```

Am modificat fisierul `shared.h` sa includa functiile necesare utile.

```c++
// shared.h ...
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
```

Si introducem informatia data in exercitiu in `main.cpp` pentru a primi rezultatul.

```c++
// main.cpp
#include <stdio.h>
#include <mydefines.h>
#include "affine.h"

int main()
{
    Affine::encrypt_and_decrypt_messages({ "the", "tree", "is", "a", "good", "hiding", "place" }, 7, 5); 
    Affine::encrypt_and_decrypt_messages({ "hide", "the", "gold", "in", "the", "tree", "stump" }, 5, 8);
}
```

Rezultatul:
```
the -> ich -> the         
tree -> iuhh -> tree      
is -> jb -> is            
a -> f -> a               
good -> vzza -> good      
hiding -> cjajsv -> hiding
place -> gefth -> place   
hide -> rwxc -> hide      
the -> zrc -> the         
gold -> malx -> gold      
in -> wv -> in            
the -> zrc -> the         
tree -> zpcc -> tree      
stump -> uzeqf -> stump
```

### 3. Folosind cifrul Polybios, criptați textul clar:

> a) "the tree is a good hiding place"  
> b) "hide the gold in the tree stump"  
> Alfabetul cu care se completează tabloul pentru criptare este constituit în baza alfabetului latin  
> ABCDEFGHIJKLMNOPQRSTUVWXYZ  
> și a cuvântului cheie  
> a)ITSECURITY b)POLYBIOS  
> În tablou literele I și J sunt combinateîn una.