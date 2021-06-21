// Code by hyder308116
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
typedef struct {
    int sz;
    char **token;
} str2token;
void get_token(str2token* st, char* s) {
    int len = strlen(s);
    int last = -1; // last position of [0-9a-zA-Z] alphabet "-1" means the previous alphabet is a non [0-9a-zA-Z] alphabet
    st->token = malloc(len*sizeof(char*)/2);
    st->sz = 0;
    for(int i = 0; i <= len; i++) {
        if(isdigit(s[i]) || isalpha(s[i])) {
            if(last == -1)
                last = i;
        }
        else if(last != -1) {
            st->token[st->sz] = malloc((i-last+1)*sizeof(char));
            strncpy(st->token[st->sz], s+last, i-last);
            st->token[st->sz][i-last] = '\0';
            last = -1;
            st->sz++;
        }
    }
    st->token = realloc(st->token,st->sz*sizeof(char*));
    return;
}

str2token* readTokenFromFile(char* filename){
    FILE* fp;
    fp = fopen(filename, "r");
    long size;
    fseek(fp, 0, SEEK_END); 
    size = ftell(fp);
    fseek(fp, 0, SEEK_SET); 
    char* buf = malloc(size);
    fread(buf, 1, size, fp);
    
    str2token* st = malloc(sizeof(str2token));
    get_token(st, buf);
    fclose(fp);
    return st;
}
