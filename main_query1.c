// By: GWFrank
#include "api.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// The testdata only contains the first 100 mails (mail1 ~ mail100)
// and 2000 queries for you to debug.

#define MAGIC_MUL 8923 // 1361=>168 4583=>142 8923=>103
#define KEY_RANGE 10000019 // a large prime number
#define KEY_SIZE 55
#define MAIL_N 10005
#define GROUP_N 17


int n_mails, n_queries;
mail *mails;
query *queries;
int stk[3000][MAIL_N];

// Get index of the hash table by char
const int transfer[128] = { // 16 per line
    -1, -1, -1, -1, -1, -1, -1, -1,  -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1,  -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1,  -1, -1, -1, -1, -1, -1, -1, -1,
     0,  4,  1,  1,  1,  1,  0,  0,   0,  0, -1, -1, -1, -1, -1, -1,
    -1, 12, 11, 15,  9,  5,  5,  7,   7,  4,  1,  6,  6, 14,  3,  2,
    13,  0,  8, 16, 10,  2,  2,  3,   0,  0,  0, -1, -1, -1, -1, -1,
    -1, 12, 11, 15,  9,  5,  5,  7,   7,  4,  1,  6,  6, 14,  3,  2,
    13,  0,  8, 16, 10,  2,  2,  3,   0,  0,  0, -1, -1, -1, -1, -1
};

// Count collision times during addMailToToken()
int COLLISION_CNT = 0;

int hashStr(char* s) {
    long long int hash = 0;
    while (*s) {
        hash = (MAGIC_MUL*hash+(*s++)) % KEY_RANGE;
    }
    return hash;
}

// Linked list for storing mail ids
typedef struct __mnode__ {
    int mail_id;
    struct __mnode__* nxt;
} mnode;

typedef struct __mlist__{
    mnode* head;
} mlist;

mlist* newMlist () {
    mlist* new = malloc(sizeof(mlist));
    new->head = NULL;
    return new;
}

void mlistInsert(mlist* l, int mail_id) {
    mnode* new = malloc(sizeof(mnode));
    new->mail_id = mail_id;
    new->nxt = l->head;
    l->head = new;
}

// Hash table by chaining, use tokens as index
typedef struct __node__ {
    char key[KEY_SIZE];
    int mail_cnt; // Number of mails with this token
    int mail_arr[MAIL_N]; // Marking if a mail have this token
    mlist* mail_list; // All mails with this token (in linked list)
    struct __node__* nxt;
} node;

typedef struct __llist__ {
    node* head;
} llist;

llist* newDict() {
    // Returns a dictionary / hash table
    llist* dict = calloc(KEY_RANGE, sizeof(llist));
    // llist* dict = malloc(KEY_RANGE*sizeof(llist));
    // for (int i=0; i<KEY_RANGE; i++) {
    //     dict[i].head = NULL;
    // }
    return dict;
}

llist** initTable() {
    // Returns a table, each element is a hash table
    llist** table = malloc(GROUP_N*sizeof(llist*));
    for (int i=0; i<GROUP_N; i++) {
        table[i] = newDict();
    }
    return table;
}

node* newKey(llist* chain, char* key) {
    // Add new key
    node* new = malloc(sizeof(node));
    strcpy(new->key, key);
    new->mail_cnt = 0;
    new->mail_list = newMlist();
    memset(new->mail_arr, 0, MAIL_N);
    new->nxt = chain->head;
    chain->head = new;
    return new;
}

void addMailToToken(node* token_node, int mail_id) {
    // Add a mail_id by token
    if (!token_node->mail_arr[mail_id]) {
        token_node->mail_cnt++;
        token_node->mail_arr[mail_id] = 1;
        mlistInsert(token_node->mail_list, mail_id);
    }
}

node* getTokenNode(llist** table, char* key) {
    llist* dict = table[transfer[key[0]]];
    int idx = hashStr(key);
    node* cur_node = dict[idx].head;
    while (cur_node) {
        if (strcmp(key, cur_node->key) == 0) {
            return cur_node;
        } else {
            COLLISION_CNT+=1;
            // printf("%s %s\n", key, cur_node->key);
            cur_node = cur_node->nxt;
        }
    }
    return newKey(&dict[idx], key);
}

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

int query1_ans_arr[MAIL_N];

int main(void){
	api.init(&n_mails, &n_queries, &mails, &queries);

    llist** hashtable = initTable();
    // Build the token table here!


	for (int i = 0; i < n_queries; i++)
		if (queries[i].type == expression_match) {
            char *expr = queries[i].data.expression_match_data.expression;
            int ans_cnt = exprEval(expr, query1_ans_arr, hashtable, n_mails);
            api.answer(queries[i].id, query1_ans_arr, ans_cnt);
        }

	return 0;
}
