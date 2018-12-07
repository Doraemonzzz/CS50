#define _XOPEN_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <math.h>

//数字字母个数25*2 = 52
const int K = 52;
//数组长度
const int M = 5E8;
//最长长度+1
const int m = 5;
//存放单词的数组，注意第二个维度长度为m+1
char words[M][5];

//获得大小写字符
void getcharacter(char c[]);
//获得单词，全存在一个数组里,start到end为长度为l的字符，返回长度l+1的起始处
int getword(char word[][m], int start, int end, char c[], int l);
//将字符s第一位添加c，存入word中，例如a,pple变为apple
void helper(char c, char s[], char word[]);

int main(int argc, string argv[]){
    char salt[3];
    string hash;
    //error
    if(argc != 2){
        printf("Usage: ./crack hash\n");
        return 1;
    }else{
        //获得salt
        salt[0] = argv[1][0];
        salt[1] = argv[1][1];
        salt[2] = '\0';
        hash = argv[1];
    }

    //获得大小写字符
    char c[K];
    getcharacter(c);

    //获得长度为1的单词
    getword(words, 0, 0, c, 1);
    int start = 0;
    int end = K-1;
    //获得其余长度的单词
    for(int i=2; i<m; i++){
        int temp = getword(words, start, end, c, i);
        start = end+1;
        end = temp;
    }

    //验证
    for(int i=0; i<end; i++){
        string Hash = crypt(words[i], salt);
        if(strcmp(Hash, hash) == 0){
            printf("%s\n", words[i]);
            break;
        }
    }

    return 0;
}


void getcharacter(char c[]){
    int cnt = 0;
    for(int i=0; i<26; i++,cnt++){
        c[cnt] = 'a' + i;
    }

    for(int i=0; i<26; i++,cnt++){
        c[cnt] = 'A' + i;
    }
}

int getword(char word[][m], int start, int end, char c[], int l){
    if(l == 1){
        for(int i=0; i<K; i++){
            char newword[2];
            newword[0] = c[i];
            newword[1] = '\0';
            strcpy(word[i], newword);
        }
        return K;
    }else{
        int cnt = 1;
        for(int i=start; i<=end; i++){
            for(int j=0; j<K; j++){
                char newword[l+1];
                helper(c[j], word[i], newword);
                strcpy(word[end+cnt], newword);
                cnt++;
            }
        }
        return end+cnt-1;
    }
}


void helper(char c, char s[],char word[]){
    int n = strlen(s) + 2;
    word[n-1] = '\0';
    word[0] = c;
    for(int i=1; i<n-1; i++){
        word[i] = s[i-1];
    }
}