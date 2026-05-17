// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dictionary.h"
int s = 0;
// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;

// TODO: Choose number of buckets in hash table
const unsigned int N = 26 * 26;

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // TODO
    node *buffer;
    char tmp[LENGTH + 1];
    strcpy(tmp, word);
    for (int i = 0; tmp[i] != '\0'; i++)
        tmp[i] = tolower(tmp[i]);
    if (table[hash(word)] == NULL)
        return false;
    buffer = table[hash(word)];
    while (0 == 0)
    {
        if (strcmp(tmp, buffer->word) == 0)
        {
            return true;
        }
        if (buffer->next == NULL)
            break;
        buffer = buffer->next;
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO: Improve this hash function
    if (word[1] != '\0' && isalpha(word[1]))
        return (toupper(word[0]) - 'A') + (toupper(word[1]) - 'A') * 26;
    else
        return toupper(word[0]) - 'A';
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // TODO
    FILE *dict = fopen(dictionary, "r");
    if (dict == NULL)
        return false;

    char wd[46];

    node *buffer = malloc(sizeof(node));
    if (buffer == NULL)
    {
        fclose(dict);
        return false;
    }
    while (fscanf(dict, "%s", wd) != EOF)
    {
        s++;
        node *n = malloc(sizeof(node));
        if (n == NULL)
        {
            fclose(dict);
            free(buffer);
            return false;
        }
        n->next = NULL;
        strcpy(n->word, wd);
        int index = hash(n->word);
        if (table[index] == NULL)
        {
            table[index] = n;
            n->next = NULL;
        }
        else
        {
            buffer->next = table[index]->next;
            n->next = buffer->next;
            table[index]->next = n;
        }
    }
    free(buffer);
    fclose(dict);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    return s;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // TODO
    for (int i = 0; i < N; i++)
    {
        node *buffer = table[i];
        while (buffer != NULL)
        {
            node *buf = buffer;
            buffer = buffer->next;
            free(buf);
        }
        table[i] = NULL;
    }
    return true;
}
