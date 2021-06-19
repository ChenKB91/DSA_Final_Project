#include <stdio.h>
#include <stdlib.h>

// typedef struct __node__ {
//     char key[KEY_SIZE];
//     int mail_cnt; // Number of mails with this token
//     int mail_arr[MAIL_N]; // Marking if a mail have this token
//     mlist* mail_list; // All mails with this token (in linked list)
//     struct __node__* nxt;
// } node;

// typedef struct __llist__ {
//     node* head;
// } llist;

// node* getTokenNode(llist** table, char* key);
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
        printf("%c\n", *expr);
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

int main(){
    char expr[2048] = "(!chard)|(pontifex)";
    char* pst;
    pst = toPostfix(expr);
    printf("%s\n", pst);
    return 0;
}