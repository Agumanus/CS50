#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

bool only_digits(string s);
char rotate(char c, int i);

int main(int argc, string argv[]) 
{
    if (argc != 2)
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }
    
    if (!only_digits(argv[1]))
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }

    int key = atoi(argv[1]);

    string plainText = get_string("plaintext:  ");
    
    printf("ciphertext: ");
    for (int i = 0, n = strlen(plainText); i < n; i++)
    {
        char cText = rotate(plainText[i], atoi(argv[1]));
        printf("%c", cText);
    }
    printf("\n");
    return 0;
}

bool only_digits(string s)
{
    for (int i = 0, n = strlen(s); i < n; i++)
    {
        if (!isdigit(s[i]))
        {
            return false;
            break;
        }
    }
    return true;
}

char rotate(char ch, int in)
{
    if (islower(ch))
    {
        ch -= 97;
        ch = (ch + in) % 26;
        ch += 97;

    }
    else if (isupper(ch))
    {
        ch -= 65;
        ch = (ch + in) % 26;
        ch += 65;
    }
    return ch;
}