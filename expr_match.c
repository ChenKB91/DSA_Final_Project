#include <stdio.h>
#include <stdlib.h>
#include "hash_table.h"

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

void eval(char *expr, int* ans_arr, llist**table) {
    char *postfix = toPostfix(expr);
    char *c = postfix;
    int stk[3000][MAIL_N];
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
                tp++; c++;
                node* buf_node = getTokenNode(table, buf);
                memcpy(stk[tp], buf_node->mail_arr, MAIL_N);
                break;
        }
    }
    memcpy(ans_arr, stk, MAIL_N);
    free(postfix);
}

int main(){
    char expr[2048] = "(!chard)|(pontifex)";
    char* pst;
    pst = toPostfix(expr);
    printf("%s\n", pst);
    return 0;
}