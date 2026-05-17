#include <cs50.h>
#include <stdio.h>

int block(int h);
int space(int s);

int main()
{
    /*getting user intput*/
    int height;
    do
    {
        height = get_int("Enter the height of the pyramid here: ");
    }
    while (height < 1);
    /*printing the pyramid*/
    for (int j = 0; j < height; j++)
    {
        space(height - j - 1);
        block(j);
        printf("  ");
        block(j);
        printf("\n");
    }
}
/*here h is 0*/
int block(int h)
{
    for (int i = 0; i <= h; i++)
    {
        printf("#");
    }
    return (0);
}

/*here s is 0*/
int space(int s)
{
    for (int i = 1; i <= s; i++)
    {
        printf(" ");
    }
    return (0);
}
