#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>

void getkey(string key, int x[]);
string vigenere(string word, int key[], int n);
bool judge(string word);

int main(int argc, string argv[]){
    string key;
    //error
    if(argc != 2){
        printf("Usage: ./vigenere k\n");
        return 1;
    }else{
        key = argv[1];
        if(judge(key)){
            printf("Usage: ./vigenere k\n");
            return 1;
        }
    }

    int n = strlen(key);
    int key1[n];
    getkey(key, key1);

    //get word
    string word1 = get_string("plaintext:  ");

    //process
    string word2 = vigenere(word1, key1, n);
    printf("ciphertext: %s\n", word2);

    return 0;
}


void getkey(string key, int x[]){
    for(int i=0, n = strlen(key); i<n; i++){
        if(islower(key[i])){
            x[i] = (key[i] - 'a');
        }else{
            x[i] = (key[i] - 'A');
        }
    }
}

string vigenere(string word, int key[], int n){
    int l = strlen(word);
    int j = 0;
    for(int i=0; i<l; i++){
        if(islower(word[i])){
            word[i] = 'a' + (word[i] - 'a' + key[j%n]) % 26;
            j++;
        }else if(isupper(word[i])){
            word[i] = 'A' + (word[i] - 'A' + key[j%n]) % 26;
            j++;
        }
    }

    return word;
}

bool judge(string word){
    int n = strlen(word);
    for(int i=0; i<n; i++){
        if((! (word[i] >='a' && word[i] <='z')) && (! (word[i] >='A' && word[i] <='Z'))){
            return true;
        }
    }
    return false;
}