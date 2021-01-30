#include <stdio.h>
#include <mydefines.h>
#include "caesar.h"
#include "affine.h"
#include "playwright.h"

int main()
{
    {
        const char* encrypted = "rkilusop";
        const char* codeword = "martor";

        const char* encryption_key = Caesar::make_encryption_key(codeword);
        const char* decryption_key = Caesar::make_decryption_key(encryption_key);
        
        char decrypted_message[sizeof(encrypted)];
        Caesar::decrypt(decrypted_message, encrypted, decryption_key);
        puts(decrypted_message);
    }
    {
        Affine::encrypt_and_decrypt_messages({ "the", "tree", "is", "a", "good", "hiding", "place" }, 7, 5); 
        Affine::encrypt_and_decrypt_messages({ "hide", "the", "gold", "in", "the", "tree", "stump" }, 5, 8);
    }
    {
        // Currently, it leaks memory for strings and vectors.
        // auto key = Playwright::make_key_default("MURPHY_");

        Playwright::Make_Params mp;
        mp.codeword = "MURPHY_";
        mp.code_positions = { 6, 5, 0, 9, 7, 4, 2 };
        mp.order = { 6, 5, 1, 0, 9, 7, 4, 2, 3, 8 };
        mp.dim = 10;
        mp.char_set = "ABCDEFGIJKLNOQSTVWXZ0123456789";

        auto key = Playwright::make_key(mp);
        Playwright::print_key(key);

        const char* message = "CLOCKS_WILL_RUN_MORE_QUICKLY_DURING_FREE_TIME";
        printf("Original message:  %s\n", message);

        auto encrypted = Playwright::encrypt(message, key);
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
        
        auto decrypted = Playwright::decrypt(encrypted, key);
        printf("Decrypted message: %s\n", decrypted);

        Playwright::destroy_key(key);
    }
}