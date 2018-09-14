#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

string caesar(string word, int k);

int main(int argc, string argv[]){
    int k;
    //error
    if(argc != 2){
        printf("Usage: ./caesar k\n");
        return 1;
    }else{
        k = atoi(argv[1]);
    }

    //get word
    string word1 = get_string("plaintext:  ");
    //process
    string word2 = caesar(word1, k);
    printf("ciphertext: %s\n", word2);

    return 0;
}

string caesar(string word, int k){
    int n = strlen(word);
    for(int i=0; i<n; i++){
        if(islower(word[i])){
            word[i] = 'a' + (word[i] - 'a' + k) % 26;
        }else if(isupper(word[i])){
            word[i] = 'A' + (word[i] - 'A' + k) % 26;
        }
    }

    return word;
}