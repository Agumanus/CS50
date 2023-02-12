#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

int count_letters(string text);
int count_words(string text);
int count_sentences(string text);

int main(void)
{
    string userText = get_string("Text: ");
    int numL = count_letters(userText);
    int numW = count_words(userText);
    int numS = count_sentences(userText);
    
    float L =   (float) numL / (float) numW * 100;
    float S =   (float) numS / (float) numW * 100;
    int index = round(0.0588 * L - 0.296 * S - 15.8);  
    
    if (index >= 16)
    {
        printf("Grade 16+\n");
    }
    else if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    else
    {
        printf("Grade %i\n", index);
    }
    
}

int count_letters(string text)
{
    int letters = 0;
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        if (isalpha(text[i]))
        {
            letters ++;
        }
    }
    return letters;
}

int count_words(string text)
{
    int words = 1; //+ 1 for the last word in text 
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        if (text[i] == 32) // 32 Ascii for space
        {
            words ++;
        }
    }
    return words;
}
int count_sentences(string text)
{
    int sentences = 0;
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        if (text[i] == 33 || text[i] == 46 || text[i] == 63) // 33 ! 46 . 63 ?
        {
            sentences ++;
        }
    }
    return sentences;
}