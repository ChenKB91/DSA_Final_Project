// By: GWFrank
#include "hash_table.h"
#include "get_token.h"
#include <stdio.h>
#include <stdlib.h>

int precedence(char c) {
    switch (c) {
        case '(': case ')':
            return 10;
        case '!':
            return 5;
        case '&':
            return 4;
        case '|':
            return 3;
        default:
            return 0;
    }
}

char* toPostfix(char* expr) {
    char stk[2500];
    int tp = -1;
    char* postfix = malloc(5000*sizeof(char));
    int post_idx = 0;
    while (*expr) {
        switch (*expr) {
            case '(':
                stk[++tp] = *expr;
                break;
            case ')':
                while (stk[tp] != '(') {
                    postfix[post_idx++] = stk[tp--];
                }
                tp--;
                if (tp >= 0) {
                    if (stk[tp] == '!') {
                        postfix[post_idx++] = stk[tp--];
                    }
                }
                break;
            case '!': case '&': case '|':
                while (tp >= 0
                        && stk[tp] != '('
                        && ((precedence(stk[tp]) > precedence(*expr)) || (stk[tp] == *expr && *expr != '!'))) {
                    postfix[post_idx++] = stk[tp--];
                }
                stk[++tp] = *expr;
                break;
            default:
                postfix[post_idx++] = *expr;
                if (precedence(*(expr+1)) != 0)
                    postfix[post_idx++] = ',';
                break;
        }
        expr++;
    }
    while (tp >= 0) {
        postfix[post_idx++] = stk[tp--];
    }
    postfix[post_idx++] = '\0';
    return postfix;
}

int stk[3000][MAIL_N];

int exprEval(char *expr, int* ans_arr, llist**table, int mail_n) {
    char *postfix = toPostfix(expr);
    char *c = postfix;
    int tp=-1;
    while (*c) {
        char buf[55];
        int idx=0;
        switch (*c) {
            case '!':
                for (int i=0; i<MAIL_N; i++)
                    stk[tp][i] = 1-stk[tp][i];
                break;
            case '&':
                for (int i=0; i<MAIL_N; i++)
                    stk[tp-1][i] = (stk[tp][i]&&stk[tp-1][i]);
                tp--;
                break;
            case '|':
                for (int i=0; i<MAIL_N; i++)
                    stk[tp-1][i] = (stk[tp][i]||stk[tp-1][i]);
                tp--;
                break;
            default:
                while (*c != ',')
                    buf[idx++] = *c++;
                buf[idx] = '\0';
                tp++;
                node* buf_node = getTokenNode(table, buf);
                memcpy(stk[tp], buf_node->mail_arr, MAIL_N);
                idx=0;
                break;
        }
        c++;
    }
    memcpy(ans_arr, stk, MAIL_N);
    int cnt=0;
    for (int i=0; i<mail_n; i++) {
        if (stk[0][i])
            ans_arr[cnt++] = i;
    }
    free(postfix);
    return cnt;
}

typedef struct mail {
	int id;
	char from[32], to[32];
	char subject[256], content[100000];
} mail;

int main(){
    char expr[2048] = "!((clementson)|(!amenities)&(!montitvo))";
    char key1[50] = "clementson";
    char key2[50] = "amenities";
    char key3[50] = "montitvo";
    // char key4[50] = "d";
    // char key5[50] = "e";
    // char key6[50] = "f";
    llist** hashtable = initTable();
    int* null_cnt = 0;
    node* token_node = getTokenNode(hashtable, key1);
    addMailToToken(token_node, 1, null_cnt);
    addMailToToken(token_node, 3, null_cnt);
    addMailToToken(token_node, 5, null_cnt);
    addMailToToken(token_node, 7, null_cnt);
    token_node = getTokenNode(hashtable, key2);
    addMailToToken(token_node, 2, null_cnt);
    addMailToToken(token_node, 3, null_cnt);
    addMailToToken(token_node, 6, null_cnt);
    addMailToToken(token_node, 7, null_cnt);
    token_node = getTokenNode(hashtable, key3);
    addMailToToken(token_node, 4, null_cnt);
    addMailToToken(token_node, 5, null_cnt);
    addMailToToken(token_node, 6, null_cnt);
    addMailToToken(token_node, 7, null_cnt);

    return 0;
}