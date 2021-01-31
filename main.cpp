#include <stdio.h>
#include <mydefines.h>
#include "caesar.h"
#include "affine.h"
#include "straddling.h"
#include "playfair.h"

int main()
{
    {
        const char* message = "CLOCKSWILLRUNMOREQUICKLYDURINGFREETIME";
        const char* keyword = "MURPHY";
        
        if (validate(keyword) != 0)
        {
            printf("The keyword is not valid: The character '%c' is not in the alphabet.", *validate(keyword));
            exit(-1);
        }
        if (validate(message) != 0)
        {
            printf("The message is not valid: The character '%c' is not in the alphabet.", *validate(message));
            exit(-1);
        }
        
        printf("Polybios key:\n");
        auto key = Playfair::make_key(5, keyword);
        Playfair::print_key(key);

        printf("Initial message: %s\n", message);

        const char* encrypted = Playfair::encrypt(message, key);
        printf("Encrypted message: %s\n", encrypted);

        const char* decrypted = Playfair::decrypt(encrypted, key);
        printf("Decrypted message: %s\n", decrypted);

        free((void*)encrypted); 
        free((void*)decrypted);
    }
    {
        const char* encrypted = "rkilusop";
        const char* keyword = "martor";

        const char* encryption_key = Caesar::make_encryption_key(keyword);
        const char* decryption_key = Caesar::make_decryption_key(encryption_key);
        
        const char* decrypted = Caesar::decrypt(encrypted, decryption_key);
        printf("Caesar: %s -> %s", encrypted, decrypted);

        free((void*)encryption_key);
        free((void*)decryption_key);
        free((void*)decrypted);
    }
    {
        Affine::encrypt_and_decrypt_messages({ "the", "tree", "is", "a", "good", "hiding", "place" }, 7, 5); 
        Affine::encrypt_and_decrypt_messages({ "hide", "the", "gold", "in", "the", "tree", "stump" }, 5, 8);
    }
    {
        // Currently, it leaks memory for strings and vectors.
        // auto key = Straddling::make_key_default("MURPHY_");

        Straddling::Make_Params mp;
        mp.keyword = "MURPHY_";
        mp.code_positions = { 6, 5, 0, 9, 7, 4, 2 };
        mp.order = { 6, 5, 1, 0, 9, 7, 4, 2, 3, 8 };
        mp.dim = 10;
        mp.char_set = "ABCDEFGIJKLNOQSTVWXZ0123456789";

        auto key = Straddling::make_key(mp);
        Straddling::print_key(key);

        const char* message = "CLOCKS_WILL_RUN_MORE_QUICKLY_DURING_FREE_TIME";
        printf("Original message:  %s\n", message);

        auto encrypted = Straddling::encrypt(message, key);
        printf("Encrypted message: "); 
        {
            const int block_size = 5;
            for (int i = 0; i < encrypted.size(); i ++)
            {
                printf("%i", encrypted[i]);
                if (i % block_size == block_size - 1)
                {
                    printf(" ");
                }
            }
        }
        printf("\n");
        
        auto decrypted = Straddling::decrypt(encrypted, key);
        printf("Decrypted message: %s\n", decrypted);

        Straddling::destroy_key(key);
    }
}