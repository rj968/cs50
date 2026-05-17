#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 1. get a 26 letter key from the user and convert them to upper/lower for ease.
//    Make sure that the input is all alpha by isalpha and check if any character
//    is repeated or not.
// 2. use the inputed key as an index and substitute them for the character in
//    in alpha bet and also +32 to them for lower case letters.
// 3. Encrpyt the plain text inputted by the user and return it as crypted text
//    after substitution.
//              ALLL THE BEST RJ :D

int main(int argc, string argv[])
{
    string key = argv[1];

    if (argc != 2)
    {
        printf("Invalid input.\n");
        return 1;
    }
    if (strlen(key) != 26)
    {
        printf("Invalid input. 1\n");
        return 1;
    }

    for (int i = 0; key[i] != '\0'; i++)
    {
        if (!isalpha(key[i]))
        {
            printf("Invalid input. 2\n");
            return 1;
        }
        else
        {
            for (int j = 0; j < 26; j++)
                if (i != j && tolower(key[i]) == tolower(key[j]))
                {
                    printf("Invalid input 3.\n");
                    return 1;
                }
        }
    }

    string text = get_string("plaintext:  ");
    string abc = "abcdefghijklmnopqrstuvwxyz";

    for (int i = 0; text[i] != '\0'; i++)
    {
        if (isalpha(text[i]))
        {
            if (islower(text[i]))
            {
                for (int j = 0; j < 26; j++)
                {
                    if (text[i] == abc[j])
                    {
                        text[i] = tolower(key[j]);
                        j = j + 26;
                    }
                }
            }
            else
            {
                for (int j = 0; j < 26; j++)
                {
                    if (text[i] == toupper(abc[j]))
                    {
                        text[i] = toupper(key[j]);
                        j = j + 26;
                    }
                }
            }
        }
    }
    printf("ciphertext: %s\n", text);
    return 0;
}
