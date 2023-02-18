'''mario.py prompts user for pyramid's height and prints it'''

from cs50 import get_int

def pyramid(h):
    if h < 1:
        return 
    pyramid(h -1)
    print((height - h) * " ", end='')
    for i in range(h):
        print("#", end='')
    print("  ",end='')
    for i in range(h):
        print("#", end='')
    print()

while True:
    height = int(get_int("height: "))
    if height > 0 and height < 9:
        break
pyramid(height)