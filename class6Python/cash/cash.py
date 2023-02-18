''' cash.py - prompts user for change and outputs minimum number of coins'''

from cs50 import get_float

def main():
    while True:
        change = get_float("Change: ")
        if change > -1:
            break
    
    numQuarters = calculate_quarters(change)
    change = round(change - (numQuarters * 0.25), 2)
    numDimes = calculate_dimes(change)
    change = round(change - (numDimes * 0.1), 2)
    numNickles = calculate_nickles(change)
    change = round(change - (numNickles * 0.05), 2)
    numPennies = calculate_pennies(change)
    change = round(change - (numPennies * 0.01), 2)

    numCoins = numQuarters + numDimes + numNickles + numPennies

    print(numCoins)

def calculate_quarters(change):
    quarters = int(change / 0.25)
    return quarters

def calculate_dimes(change):
    dimes = int(change / 0.1)
    return dimes

def calculate_nickles(change):
    nickles = int(change / 0.05)
    return nickles

def calculate_pennies(change):
    pennies = int(change / 0.01)
    return pennies

if __name__ == "__main__":
    main()