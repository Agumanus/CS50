# include <cs50.h>
# include <stdio.h>

int main(void)
{
    string userName = get_string("Hi! What's your name? ");
    printf("Nice to meet you, %s!\n", userName); 
}
