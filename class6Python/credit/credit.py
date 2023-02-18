'''credit.py - checks validity of credit card number'''

from cs50 import get_string
import re

def main():
    while True:
        # prompt user for cc number
        cc = get_string("Number: ")
        # check if digit-only with regex
        ccRegex = re.compile(r'^\d+$')
        if ccRegex.search(cc) != None:
            break
    # check syntax of cc via Luhn
    if checkSum(cc):
        # check cc type
        cc_type(cc)
    else:
        print("Invalid")

def checkSum(cc):
    sum = 0
    
    # multiply every other digit by two, starting with the second to last number
    pointer = -2
    while (pointer * -1) <= len(cc):
        product = int(cc[pointer]) * 2
        # add those products' digits together 
        if len(str(product)) < 2:
            sum += product
        else:
            for i in range(len(str(product))):
                sum += int(str(product)[i])
        pointer -= 2
    
    # add the sum to the sum of digits that weren't multiplied by two
    pointer = -1
    while (pointer * -1) <= len(cc):
        sum += int(cc[pointer])
        pointer -= 2

    # if the total modulo 10 is congruent to 0, the number is valid
    if sum % 10 == 0:
        return True
    else:
        return False

def cc_type(cc):
    # Amex
    if len(cc) == 15 and cc[0] == '3' and (cc[1] == '4' or cc[1] == '7'):
        print("AMEX")
    # Visa
    elif (len(cc) == 13 or len(cc) == 16) and cc[0] == '4':
        print("VISA")
    # Master
    elif len(cc) == 16 and cc[0] == '5' and cc[1] in ['1', '2', '3', '4', '5']:
        print("MASTER")
    else:
        print("Invalid")

if __name__ == "__main__":
    main()