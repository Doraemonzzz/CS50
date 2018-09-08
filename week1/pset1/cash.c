#include <stdio.h>
#include <cs50.h>
#include <math.h>

int f(int n);

int main()
{
    float m;
    do
    {
        m = get_float("Change owed: ");
    }while(m < 0);

    int n = round(m*100);
    int s = f(n);
    printf("%i\n", s);
}

int f(int n)
{
    //printf("%i\n", n);
    if(n == 0)
    {
        return 0;
    }
    else if((n == 1) || (n == 5) || (n == 10) || (n == 25))
    {
        return 1;
    }
    else if(n < 5)
    {
        return 1 + f(n - 1);
    }
    else if((n >= 5) && (n < 10))
    {
        return 1 + f(n - 5);
    }
    else if((n >= 10) && (n < 25))
    {
        return 1 + f(n - 10);
    }
    else
    {
        return 1 + f(n - 25);
    }
}