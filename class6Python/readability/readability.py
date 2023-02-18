'''readabilty.py - prompts user for text and outputs its grade level'''

from cs50 import get_string

def main():
    text = get_string("Text: ")
    
    letters = count_letters(text)
    words = count_words(text)
    sentences = count_sentences(text)

    L = letters / words * 100
    S = sentences / words * 100
    
    index = round(0.0588 * L - 0.296 * S - 15.8) 

    if index > 15:
        print("Grade 16+")
    elif index < 1:
        print("Before Grade 1")
    else:
        print(f"Grade {index}")

def count_letters(text):
    letters = 0
    for i in range(len(text)):
        if text[i].isalpha():
            letters += 1
    return letters

def count_words(text):
    # + 1 for the last word in text
    words = 1
    for i in range(len(text)):
        if text[i] == " ":
            words += 1
    return words

def count_sentences(text):
    sentences = 0
    for i in range(len(text)):
        if text[i] == "." or text[i] == "!" or text[i] == "?":
            sentences += 1
    return sentences

if __name__ == "__main__":
    main()