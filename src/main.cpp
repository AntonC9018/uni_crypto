#include <stdio.h>
#include <mydefines.h>
#include <strlib.h>
#include <gtkmm/application.h>
#include "modern/gui/window.h"



int main()
{
    auto app = Gtk::Application::create("cryto.demo.app");
    ModernMainWindow window;
    int status = app->run(window);
    printf("Error code: %i", status);
    return status;

    // {
    //     auto message = str_lit("CLOCKSWILLRUNMOREQUICKLYDURINGFREETIME");
    //     auto keyword = str_lit("MURPHY");
        
    //     if (validate(keyword) != 0)
    //     {
    //         printf("The keyword is not valid: The character '%c' is not in the alphabet.", *validate(keyword));
    //         exit(-1);
    //     }
    //     if (validate(message) != 0)
    //     {
    //         printf("The message is not valid: The character '%c' is not in the alphabet.", *validate(message));
    //         exit(-1);
    //     }
        
    //     printf("Polybios key:\n");
    //     auto key = Playfair::make_key(5, keyword);
    //     Playfair::print_key(key);

    //     printf("Initial message: %s\n", message.chars);

    //     auto encrypted = Playfair::encrypt(message, key);
    //     printf("Encrypted message: %s\n", encrypted.chars);

    //     auto decrypted = Playfair::decrypt(str_view(encrypted), key);
    //     printf("Decrypted message: %s\n", decrypted.chars);

    //     str_free(encrypted); 
    //     str_free(decrypted);
    // }
    // {
    //     auto encrypted = str_lit("rkilusop");
    //     auto keyword =   str_lit("martor");

    //     auto encryption_key = Caesar::make_encryption_key(keyword);
    //     auto decryption_key = Caesar::make_decryption_key(str_view(encryption_key));
        
    //     auto decrypted = Caesar::decrypt(encrypted, str_view(decryption_key));
    //     printf("Caesar: %s -> %s\n", encrypted.chars, decrypted.chars);

    //     str_free(encryption_key);
    //     str_free(decryption_key);
    //     str_free(decrypted);
    // }
    // {
    //     Affine::encrypt_and_decrypt_messages(
    //     { 
    //         str_lit("the"), str_lit("tree"), str_lit("is"), 
    //         str_lit("a"), str_lit("good"), str_lit("hiding"), 
    //         str_lit("place")
    //     }, 7, 5); 

    //     Affine::encrypt_and_decrypt_messages(
    //     { 
    //         str_lit("hide"), str_lit("the"), str_lit("gold"), 
    //         str_lit("in"), str_lit("the"), str_lit("tree"), 
    //         str_lit("stump")
    //     }, 5, 8);
    // }
    // {
    //     auto key = Straddling::make_key(str_lit("MURPHY_"), { 1, 3, 8 }, str_lit("PLAYWRIGHT"));
    //     Straddling::print_key(key);

    //     auto message = str_lit("CLOCKS_WILL_RUN_MORE_QUICKLY_DURING_FREE_TIME");
    //     printf("Original message:  %s\n", message.chars);

    //     auto encrypted = Straddling::encrypt(message, key);
    //     printf("Encrypted message: "); 
    //     {
    //         const size_t block_size = 5;
    //         for (size_t i = 0; i < encrypted.size(); i ++)
    //         {
    //             printf("%zu", encrypted[i]);
    //             if (i % block_size == block_size - 1)
    //             {
    //                 printf(" ");
    //             }
    //         }
    //     }
    //     printf("\n");
        
    //     auto decrypted = Straddling::decrypt(encrypted, key);
    //     printf("Decrypted message: %s\n", decrypted.chars);

    //     str_free(decrypted);
    //     Straddling::destroy_key(key);
    // }
    // {
    //     Shift::Key key { {5, 1, 3, 0, 4, 2}, {2, 0, 3, 1, 4} };

    //     auto message = str_lit("NuLasaPeMaineCePotiFaceAzi");
    //     printf("Original message:  %s\n", message.chars);

    //     auto encrypted = Shift::encrypt(message, key);
    //     printf("Encrypted message: %s\n", encrypted.chars);

    //     auto decrypted = Shift::decrypt(str_view(encrypted), key);
    //     printf("Decrypted message: %s\n", decrypted.chars);
        
    //     str_free(encrypted);
    //     str_free(decrypted);
    //     Shift::destroy_key(key);
    // }
    // {
    //     Vigenere::Key key { str_lit("battista") };

    //     auto message = str_lit("asimpleexample");
    //     printf("Original message:  %s\n", message.chars);

    //     auto encrypted = Vigenere::encrypt(message, key);
    //     printf("Encrypted message: %s\n", encrypted.chars);

    //     auto decrypted = Vigenere::decrypt(str_view(encrypted), key); 
    //     printf("Decrypted message: %s\n", decrypted.chars);

    //     str_free(encrypted);
    //     str_free(decrypted);
    // }
    // {
    //     auto key = Bazeries::make_key(str_lit("SEVENTHOUSANDTHREEHUNDREDANDFIFTYTWO"), { 7, 3, 5, 2 });

    //     auto message = str_lit("ACLEARCONSCIENCEISUSUALLYTHESIGNOFABADMEMORY");
    //     printf("Original message:  %s\n", message.chars);

    //     auto encrypted = Bazeries::encrypt(message, key);
    //     printf("Encrypted message: %s\n", encrypted.chars);

    //     auto decrypted = Bazeries::decrypt(str_view(encrypted), key);
    //     printf("Decrypted message: %s\n", decrypted.chars);

    //     Bazeries::destroy_key(key);
    //     str_free(encrypted);
    //     str_free(decrypted);
    // }
}