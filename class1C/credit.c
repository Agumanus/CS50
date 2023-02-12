#include <cs50.h>
#include <stdio.h>

long ccLength(long cc);
long checkSum(long cc);
long typeCC(long cc);

int main(void) 
{
    long ccNumber = get_long("Which number would you like to be checked? ");

    // get length of cc number
    int length = ccLength(ccNumber);
    
    // validate checkSum
    int luhn = checkSum(ccNumber);

    // get first two digits
    int ccType = typeCC(ccNumber);
    
    if (luhn % 10 == 0 && length > 12)
    {
        //Visa
        if (length > 12 && length < 17 && ((ccType - (ccType % 10)) / 10) % 4 == 0)
        {
            printf("VISA\n");
        }
        //Master
        else if (length == 16 && ((ccType - (ccType % 10)) / 10) % 5 == 0 && (ccType % 10) > 0 && (ccType % 10) < 6) 
        {
            printf("MASTER\n");
        }
        //Amex
        else if (length == 15 && ((ccType - (ccType % 10)) / 10) % 3 == 0 && (ccType % 10) == 4 || (ccType % 10) == 7)
        {
            printf("AMEX\n");
        }
        else
        {
            printf("INVALID\n");
        }
    }
    else
    {
        printf("INVALID\n");
    }
}

long ccLength(long cc)
{
    long lengthCC = 1;
    long a = 10;
    while (cc % a != cc)
    {
        lengthCC ++;
        a *= 10;
    }
    return lengthCC;
}

long checkSum(long cc)
{
    long x = 10;
    long oddDigit = 0;
    long evenDigit = 0;

    //switch
    int s = 0;
    
    // first cc digit
    long ccDigit = cc % 10;
    oddDigit += ccDigit;
    
    while (cc % x != cc)
    {
         // get every single cc digit
         x *= 10;
         ccDigit = (((cc % x) - (cc % (x/10))) / (x/10));
         
         // separate cc digits in two sums
         if (s == 0)
         {
            // ?add product digits
            int zahl = ccDigit * 2;
            if (zahl / 10 < 1)
            {
                evenDigit += zahl;
            }
            else
            {
                evenDigit += (zahl % 10) + 1;
            }
            s = 1;
         }
         else
         {
            oddDigit += ccDigit;
            s = 0;
         }
    }
    int checkSum = oddDigit + evenDigit;
    int checkSumResult = checkSum % 10;
    return checkSumResult;
}

long typeCC(long cc)
{
    long x = 10;
    int firstDigit = 0;
    int secondDigit = 0;
    while (cc % x != cc)
    {
        x *= 10;
        if (cc % x == cc)
        { 
            firstDigit = (((cc % x) - (cc % (x/10))) / (x/10));
            secondDigit = (((cc % (x/10)) - (cc % (x/100))) / (x/100));
            
        }
    }
    int typeDigits = firstDigit * 10 + secondDigit;
    return typeDigits;
}
