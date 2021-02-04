#include <stdio.h>
#include <mydefines.h>
#include "algos/caesar.h"
#include "algos/affine.h"
#include "algos/straddling.h"
#include "algos/playfair.h"

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

        str_free(encrypted); 
        str_free(decrypted);
    }
    {
        const char* encrypted = "rkilusop";
        const char* keyword = "martor";

        const char* encryption_key = Caesar::make_encryption_key(keyword);
        const char* decryption_key = Caesar::make_decryption_key(encryption_key);
        
        const char* decrypted = Caesar::decrypt(encrypted, decryption_key);
        printf("Caesar: %s -> %s", encrypted, decrypted);

        str_free(encryption_key);
        str_free(decryption_key);
        str_free(decrypted);
    }
    {
        Affine::encrypt_and_decrypt_messages({ "the", "tree", "is", "a", "good", "hiding", "place" }, 7, 5); 
        Affine::encrypt_and_decrypt_messages({ "hide", "the", "gold", "in", "the", "tree", "stump" }, 5, 8);
    }
    {
        auto key = Straddling::make_key("MURPHY_", { 1, 3, 8 }, "PLAYWRIGHT");
        Straddling::print_key(key);

        const char* message = "CLOCKS_WILL_RUN_MORE_QUICKLY_DURING_FREE_TIME";
        printf("Original message:  %s\n", message);

        auto encrypted = Straddling::encrypt(message, key);
        printf("Encrypted message: "); 
        {
            const size_t block_size = 5;
            for (size_t i = 0; i < encrypted.size(); i ++)
            {
                printf("%zu", encrypted[i]);
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