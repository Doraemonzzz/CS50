// Helper functions for music

#include <cs50.h>

#include "helpers.h"
#include <string.h>
#include <math.h>

// Converts a fraction formatted as X/Y to eighths
//X/Y/(1/8) = 8X/Y
int duration(string fraction)
{
    // TODO
    char a = fraction[0];
    char b = fraction[2];
    int X = a - '0';
    int Y = b - '0';
    return 8 * X / Y;
}

// Calculates frequency (in Hz) of a note
// A4 = 440;
int frequency(string note)
{
    // TODO
    // F#4
    char a, b, c;
    if(strlen(note) == 3){
        a = note[0];
        b = note[1];
        c = note[2];
    }else{
        a = note[0];
        b = ' ';
        c = note[1];
    }

    //STEP 1:转化为ab4;
    double result = 440;
    int d = c - '4';
    result *= pow(2, d);
    // if(d<0){
    //     for(int i=0; i<-d; i++){
    //         result /= 2;
    //     }
    // }else{
    //     for(int i=0; i<d; i++){
    //         result *= 2;
    //     }
    // }

    //STEP 2:计算ab与A的距离
    int e = 0;
    if(a == 'A'){

    }else if(a == 'B'){
        e += 2;
    }else if(a  >= 'F'){
        e = 2*(a - 'G' - 1);
    }else{
        e = -5 + 2*(a - 'E');
    }
    if(b == 'b'){
        e -= 1;
    }else if( b == '#'){
        e += 1;
    }

    result *= pow(2, e/12.0);

    return (int) (result + 0.5);

}

// Determines whether a string represents a rest
bool is_rest(string s)
{
    // TODO
    return strcmp(s, "") == 0;
}
