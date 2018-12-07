// Implements a dictionary's functionality

#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "dictionary.h"


int hash(const char * word){
    int s = 0;
    for(int i = 0; i<strlen(word); i++){
        s += tolower(word[i]);
    }
    return s%hashsize;
}

//定义node
typedef struct node{
    char word[LENGTH + 1];
    struct node *next;
}node;

//按照字母顺序
node *hashtable[hashsize];

//记录单词数量
int cnt = 0;

int checkword(const char * word1, const char * word2){
    int n2 = strlen(word2);
    char new_word[n2+1];
    for(int i=0; i<n2; i++){
        new_word[i] = tolower(word2[i]);
    }
    new_word[n2] = '\0';
    return strcmp(word1, new_word);
}

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    //转换为小写
    int n2 = strlen(word);
    char new_word[n2+1];
    for(int i=0; i<n2; i++){
        new_word[i] = tolower(word[i]);
    }
    new_word[n2] = '\0';

    // TODO
    int s = hash(new_word);
    //printf("%d %s\n", s, word);
    node *new_node = hashtable[s];
    while(new_node != NULL){
        //int n1 = strlen(new_node->word);
        if(strcmp(new_node->word, new_word) == 0){
            return true;
        }else{
            new_node = new_node->next;
        }
    }
    free(new_node);

    return false;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // 赋值为空指针
    for(int i=0; i<hashsize; i++){
        hashtable[i] = NULL;
    }
    FILE* fp = fopen(dictionary, "r");
    if(fp == NULL){
        printf("Can't open that file.\n");
        return false;
    }

    char a[LENGTH + 1];

    //扫描
    while (fscanf(fp, "%s", a) != EOF){
        int s = hash(a);
        node *new_node = malloc(sizeof(node));
        //没有空间
        if(new_node == NULL){
            return false;
        }
        cnt++;
        if(hashtable[s] == NULL){
            strcpy(new_node->word, a);
            new_node->next = NULL;
            hashtable[s] = new_node;
        }else{
            strcpy(new_node->word, a);
            new_node->next = hashtable[s];
            hashtable[s] = new_node;
        }
    }
    fclose(fp);

    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    return cnt;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    // TODO
    for(int i=0; i<hashsize; i++){
        node *cursor = hashtable[i];
        while(cursor != NULL){
            node *temp = cursor;
            cursor = cursor->next;
            free(temp);
        }
    }

    return true;
}
