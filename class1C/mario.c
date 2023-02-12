#include <cs50.h>
#include <stdio.h>

int main(void) 
{
    int height;
    do
    {
        height = get_int("Please enter a height between 1 and 8: ");
    }
    while (height > 8 || height < 1);    

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < (height - (i +1)); j++)
        {
            printf(" ");
        }
        // kann ich untere while in for umwandeln?
        int k = height - (i +1);
        while (k != height)
        {
            printf("#");
            k++;
        }
        printf("\n");

    }
}