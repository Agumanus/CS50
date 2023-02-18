// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include <strings.h>
#include <stdio.h>
#include <stdlib.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// variable for size function
int dictSize = 0;

// Choose number of buckets in hash table
const unsigned int N = 26;

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // hash word
    int index = hash(word);

    // access linked list at that index
    // traverse linked list strcasecmp
    
    node *cursor = table[index]; 
    
    while (strcasecmp(word, cursor->word) != 0)
    {
        if (cursor->next == NULL)
        {
            return false;
        }
        
        cursor = cursor->next;
    }
    
    return true;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // 1. First letter 
    // 26 buckets: a to z (A is 65 in ASCII)
    return toupper(word[0]) - 'A';

    // 3. Math using all the letters
    //int i = 0;
    //long wordSum = 0;
    //while (word[i] != 0)
    //{
    //    wordSum += toupper(word[i]);
    //    i++;
    //}
    //return wordSum % N;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // open dictionary file
    FILE *dictFile = fopen(dictionary, "r");
    if (dictFile == NULL)
    {
        return false;
    }
    
    // read strings from file
    char tmpWord[LENGTH + 1];
    while (fscanf(dictFile, "%s", tmpWord) != EOF)
    {
        // create a new node
        node *n = malloc(sizeof(node));
        if (n == NULL)
        {
            return false;
        }
        strcpy(n->word, tmpWord);
        //n->next = NULL;

        // hash word
        int index = hash(n->word);

        // insert node into hash table
        n->next = table[index];
        table[index] = n;

        // increase dictSize by 1
        dictSize ++;
    }
    fclose(dictFile);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return dictSize;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{

    for (int i = 0; i < N; i++)
    {
        node *cursor = table[i];
        node *tmp = table[i];

        while (cursor != NULL)
        {
            cursor = cursor->next;
            free(tmp);
            tmp = cursor;
        }
        
    }
    
    return true;
}
