// Code by hyder308116
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
typedef struct {
    int sub_sz, sz;  // size of sub_token and token
    char **sub_token; // token get from subject
    char **token; // token get from content
} str2token;
void get_token(str2token* st, mail* s) {
    int len = strlen(s->content);
    int last = -1; // last position of [0-9a-zA-Z] alphabet "-1" means the previous alphabet is a non [0-9a-zA-Z] alphabet
    st->token = malloc(len*sizeof(char*)/2);
    st->sz = 0;
    for (int i = 0; i <= len; i++) {
        if (isdigit(s->content[i]) || isalpha(s->content[i])) {
            if (last == -1)
                last = i;
        }
        else if (last != -1) {
            st->token[st->sz] = malloc((i-last+1)*sizeof(char));
            strncpy(st->token[st->sz], s->content+last, i-last);
            st->token[st->sz][i-last] = '\0';
            last = -1;
            st->sz++;
        }
    }
    st->token = realloc(st->token, st->sz*sizeof(char*));
    len = strlen(s->subject);
    last = -1;
    st->sub_token = malloc(len*sizeof(char)/2);
    st->sub_sz = 0;
    for (int i = 0; i <= len; i++) {
        if (isdigit(s->subject[i]) || isalpha(s->subject[i])) {
            if(last == -1)
                last = i;
        }
        else if (last != -1) {
            st->sub_token[st->sub_sz] = malloc((i-last+1)*sizeof(char));
            strncpy(st->sub_token[st->sub_sz], s->subject+last, i-last);
            st->sub_token[st->sub_sz][i-last] = '\0';
            last = -1;
            st->sub_sz++;
        }
    }
    st->sub_token = realloc(st->sub_token, st->sub_sz*sizeof(char));
    return;
}
