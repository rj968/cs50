#include <cs50.h>
#include <stdio.h>

int checklength(long l);
int checksum(long N);

int main(void)
{
    /*getting credit card number*/
    long number = get_long("Enter your credit card number here: ");

    /*checking if the lenght of the number is valid*/
    int length = checklength(number);

    /*calculating checksum*/
    int result = checksum(number);

    /*checking if checksum is true*/
    if (result == 0)
    {
        if (length == 13 && number / 1000000000000 == 4)
        {
            printf("VISA\n");
        }
        else if (length == 15 && (number / 10000000000000 == 34 || number / 10000000000000 == 37))
        {
            printf("AMEX\n");
        }
        else if (length == 16)
        {
            if (number / 100000000000000 == 51 || number / 100000000000000 == 52 ||
                number / 100000000000000 == 53 || number / 100000000000000 == 54 ||
                number / 100000000000000 == 55)
            {
                printf("MASTERCARD\n");
            }
            else if (number / 1000000000000000 == 4)
            {
                printf("VISA\n");
            }
            else
            {
                printf("INVALID\n");
            }
        }
        else
        {
            printf("INVALID\n");
        }
    }
    // if fake then return invalid
    else
    {
        printf("INVALID\n");
    }
}

/*calculating the length of the credit card*/
int checklength(long l)
{
    int j = 1;

    for (long i = 10; l / i > 0; i *= 10)
    {
        j++;
    }
    return (j);
}

int checksum(long N)
{
    int odd_total = 0;
    int even_total = 0;
    int counter = 0;
    for (long j = 10; N / (j / 10) > 0; j *= 10)
    {
        /* i want to find the modular of N and divide it
        from j/10 to get the digit
        and also initiate another variable such that it
        determines whether the digit is even or odd*/
        long remainder = N % j;
        int divisor = remainder / (j / 10);

        counter++;

        /*separating the odd and the even numbers and adding them together*/
        if (counter % 2 == 1)
        {
            odd_total += divisor;
        }
        else
        {
            int even_digit = 2 * divisor;
            even_total += (even_digit / 10) + (even_digit % 10);
        }
    }
    /*checksum*/
    int sum_total = even_total + odd_total;
    int check_sum = sum_total % 10;
    return (check_sum);
}

/*int last_digits(long last)
{
    for (long i = 1000000000000; last / i > 0; i *= 10)
    {}
}*/
