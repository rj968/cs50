#include <cs50.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>

int spaces(string passage);

int main(void)
{
    // getting the user input
    string passage = get_string("Enter the passage here: ");
    spaces(passage);
}

// calculating and returning grade
int spaces(string passage)
{
    float letters = 0;
    float sentences = 0;
    int spaces = 0;
    // counting letters, spaces and sentences
    for (int i = 0; passage[i] != '\0'; i++)
    {
        if (isalpha(passage[i]))
            letters++;
        if (passage[i] == ' ')
            spaces++;
        else if (passage[i] == '.' || passage[i] == '?' || passage[i] == '!')
            sentences++;
    }

    // calculating index variables
    float words = spaces + 1;
    float avg_letters = (letters / words) * 100.00;
    float avg_sentences = (sentences / words) * 100.00;

    // calculating index and rounding it to nearest integer
    float index = (0.0588 * avg_letters) - (0.296 * avg_sentences) - 15.8;
    int grade = round(index);

    // returning grade
    if (grade <= 1)
        printf("Before Grade 1\n");
    else if (grade >= 16)
        printf("Grade 16+\n");
    else
        printf("Grade %i\n", grade);
    return 0;
}
