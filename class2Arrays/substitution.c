#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

bool keyCheck(string s);
char substitute(char c, string s);

int main(int argc, string argv[]) 
{
    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }
    
    if (!keyCheck(argv[1]))
    {
        printf("length of key = 26, key is alphabetical, no duplicates\n");
        return 1;
    }

    string plainText = get_string("plaintext:  ");
    
    printf("ciphertext: ");
    for (int i = 0, n = strlen(plainText); i < n; i++)
    {
        char cTextChar = substitute(plainText[i], argv[1]);
        printf("%c", cTextChar);
    }
    printf("\n");
}

bool keyCheck(string s)
{
    // length of key must be 26
    if (strlen(s) != 26)
    {
        return false;
    }
    int keySum;
    for (int i = 0, n = strlen(s); i < n; i++)
    {
        // key must be alphabetical
        if (!isalpha(s[i]))
        {
            return false;
            break;
        }

        if (islower(s[i]))
        {
            keySum += (s[i] - 97);
        }
        else
        {
            keySum += (s[i] - 65);
        }
    }
    // sum of key characters has to be 325, 0+1+2+...+25 = 325
    if (keySum != 325)
    {
        return false;
    }
    return true;
}

char substitute(char c, string s)
{
    if (islower(c))
    {
        c -= 97;
        if (islower(s[c]))
        {
            c = s[c];
        }
        else
        {
            c = tolower(s[c]);
        }
    }
    else if (isupper(c))
    {
        c -= 65;
        if (isupper(s[c]))
        {
            c = s[c];
        }
        else
        {
            c = toupper(s[c]);
        }
    }
    return c;
}