#include <stdio.h>
#include <cs50.h>
#include <math.h>

int getlen(long int n);
int judge(long int n,int l);
int gethead(long int n, int l, int k);
int digitsum(long int n);

int main(void)
{
    long int n;
    int l;
    //get data
    do
    {
        n = get_long_long("Number: ");
    }while(n < 0);

    //get length
    l = getlen(n);

    //VISA
    if((l == 13 || l == 16) && (gethead(n, l, 1) == 4) && judge(n, l))
    {
        printf("VISA\n");
    }
    //AMEX
    else if((l == 15) && (gethead(n, l, 2) == 34 || gethead(n, l, 2) == 37) && judge(n, l))
    {
        printf("AMEX\n");
    }
    //MASTERCARD
    else if((l == 16) && ((gethead(n, l, 2) >= 51) && (gethead(n, l, 2) <= 55)) && judge(n, l))
    {
        printf("MASTERCARD\n");
    }
    else
    {
        printf("INVALID\n");
    }

    return 0;
}

int getlen(long int n)
{
    int l = 0;
    while(n > 0)
    {
        l++;
        n /= 10;
    }
    return l;
}


int gethead(long int n, int l, int k)
{
    long int s = pow(10, l - k);
    return n / s;
}

int judge(long int n, int l)
{
    int s = 0;
    for(int i=0; i<l; i++)
    {
        if(i%2 == 0)
        {
            s += (n%10);
        }
        else
        {
            s += digitsum(2*(n%10));
        }
        n /= 10;
    }


    if(s%10 == 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int digitsum(long int n)
{
    int s = 0;
    while(n > 0)
    {
        s += (n%10);
        n /= 10;
    }

    return s;
}