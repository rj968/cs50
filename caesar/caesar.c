#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

// okay I'm tired today so just lemme lay out a blueprint for the code
// 1. get a no as a input. dont accept anything else. ig use isnum() frm ctype.h for that
// 2. no % 26 = the actual key. Then use a for loop for whatever is the user input and
//    identify each letter by isalpha. then add the key to them.
//    Also figure out how to move from Z to A. -- the actual confusing part
//    IDEA - use 'Z' - 'the_letter' and then check if it is less than the key
//    if it is then subtract 'Z' from the key and start from 'A' with the new "key" we have.
//    I am a genius.
// 3. then add back the encrpted message to their original place and return as output.

int main(int argc, string argv[])
{
    if (argc != 2)
    {
        printf("Invalid input.\n");
        return 1;
    }
    string k = argv[1];
    for (int i = 0; k[i] != '\0'; i++)
    {
        if (!isdigit(k[i]))
        {
            printf("Usage: ./ceaser key\n");
            return 1;
        }
    }

    string text = get_string("Plain text:  ");

    int key = atoi(argv[1]);
    key = key % 26;

    for (int i = 0; text[i] != '\0'; i++)
    {
        if (isalpha(text[i]))
        {
            if (('Z' - toupper(text[i]) - key) >= 0)
            {
                text[i] = text[i] + key;
            }
            else
            {
                if (isupper(text[i]))
                {
                    // printf("%i ", text[i]);
                    text[i] = 'A' - ('Z' - toupper(text[i]) - key + 1);
                    // printf("%i\n", text[i]);
                }
                else
                {
                    text[i] = 'a' - ('Z' - toupper(text[i]) - key + 1);
                }
            }
        }
    }
    printf("ciphertext: %s\n", text);
}
