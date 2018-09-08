#include <stdio.h>
#include <cs50.h>

void printmario(int n);

int main(void)
{
    int n;
    while(true){
        n = get_int("Height: ");
        if(n >=0 && n <= 23){
            break;
        }
    }
    printmario(n);
}


void printmario(int n)
{
    for(int i=0; i<n; i++){
        for(int j=0; j<n-i-1; j++){
            printf(" ");
        }
        for(int j=0; j<i+1; j++){
            printf("#");
        }
        printf("  ");
        for(int j=0; j<i+1; j++){
            printf("#");
        }
        printf("\n");
    }
}