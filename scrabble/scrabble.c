#include <cs50.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

int metric(char word);
int score_counter(string word);
int compare(int a, int b);
string lower(string word);

int main(void)
{
    // getting the user input
    string word[2];
    word[0] = get_string("Player1 enter your word here: ");
    word[1] = get_string("Player2 enter your word here: ");
    // making all the letters in the word in lower case
    word[0] = lower(word[0]);
    word[1] = lower(word[1]);
    // calculating the score for each player
    int score[2];
    score[0] = score_counter(word[0]);
    score[1] = score_counter(word[1]);
    // finding the winner by comparing the scores
    compare(score[0], score[1]);

    return 0;
}
// turning all the letters to lowercase
string lower(string word)
{
    for (int i = 0; i < strlen(word); i++)
    {
        if (word[i] <= 'Z' && word[i] >= 'A')
            word[i] = word[i] + ('a' - 'A');
    }
    return word;
}
// comparing the scores to get the winner
int compare(int a, int b)
{
    if (a > b)
        printf("Player 1 wins!\n");
    else if (a < b)
        printf("Player 2 wins!\n");
    else
        printf("Tie!\n");

    return 0;
}
// calculating score
int score_counter(string word)
{
    int score = 0;
    for (int i = 0; i < strlen(word); i++)
    {
        score += metric(word[i]);
    }
    return score;
}
// defining the score metric
int metric(char character)
{
    int points = 0;
    int point[10][10] = {{'a', 'e', 'i', 'l', 'n', 'o', 'r', 's', 't', 'u'},
                         {'d', 'g'},
                         {'b', 'c', 'm', 'p'},
                         {'f', 'h', 'v', 'w', 'y'},
                         {'k'},
                         {},
                         {},
                         {'j', 'x'},
                         {},
                         {'q', 'z'}};

    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            if (character == point[i][j])
            {
                return points = i + 1;
                j = i = 10;
            }
        }
    }
    return points;
}
