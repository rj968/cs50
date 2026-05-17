#include <cs50.h>
#include <stdio.h>

/*.1 = penny, .5 = nickel, .10 = dime, .25 = quarter*/
int change(int c);

int amt;

int main(void)
{
    /*declaring all the change variables*/
    int quarter = 25;
    int dime = 10;
    int nickel = 5;
    int penny = 1;
    /*getting an integer input from the user for the amount*/
    do
    {
        amt = get_int("enter the amount of change here: ");
    }
    while (amt < 0);
    /*calculating the amount of change to be given*/
    int q = change(quarter);
    int d = change(dime);
    int n = change(nickel);
    int p = change(penny);
    /*adding up all the change*/
    int c = q + d + n + p;
    printf("%d \n", c);
}

/*function for calculating the amount of change*/
int change(int c)
{
    while (amt - c >= 0)
    {
        int i = amt / c;
        amt = amt - (i * c);
        return (i);
    }
    return (0);
}
