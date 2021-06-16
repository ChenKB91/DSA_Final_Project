#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAGIC_MUL 31
#define KEY_RANGE 1299827 // a large prime number
#define KEY_SIZE 10005
#define MAIL_N 10005

const int transfer[128] = { // 16 per line
    -1, -1, -1, -1, -1, -1, -1, -1,  -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1,  -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1,  -1, -1, -1, -1, -1, -1, -1, -1,
     0,  2,  1,  0,  0,  0,  0,  0,   0,  0, -1, -1, -1, -1, -1, -1,
    -1, 14,  8, 10,  3,  2,  6,  1,   5, 11,  0,  0,  3,  4,  1, 12,
     7,  0,  4, 13, -1,  1,  0,  9,   0,  0,  0, -1, -1, -1, -1, -1,
    -1, 14,  8, 10,  3,  2,  6,  1,   5, 11,  0,  0,  3,  4,  1, 12,
     7,  0,  4, 13, -1,  1,  0,  9,   0,  0,  0, -1, -1, -1, -1, -1
};


int hashStr(char* s) {
    long long int hash = 0;
    while (*s) {
        hash = (MAGIC_MUL*hash+(*s++)) % KEY_RANGE;
    }
    return hash;
}

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

typedef struct __node__ {
    char key[KEY_SIZE];
    int mail_cnt;
    // int mail_arr[MAIL_N];
    mlist* mail_list;
    struct __node__* nxt;
} node;

typedef struct __llist__ {
    node* head;
} llist;

llist* newDict() {
    // llist* dict = calloc(KEY_RANGE, sizeof(llist));
    llist* dict = malloc(KEY_RANGE*sizeof(llist));
    for (int i=0; i<KEY_RANGE; i++) {
        dict[i].head = NULL;
    }
    return dict;
}

llist** initTable() {
    llist** table = malloc(15*sizeof(llist*));
    for (int i=0; i<15; i++) {
        table[i] = newDict();
    }
    return table;
}

void newKey(llist** table, char* key, int mail_id) {
    llist* dict = table[transfer[key[0]]];
    node* new = malloc(sizeof(node));
    int idx = hashStr(key);
    strcpy(new->key, key);
    new->mail_cnt = 1;
    new->mail_list = newMlist();
    mlistInsert(new->mail_list, mail_id);
    new->nxt = dict[idx].head;
    dict[idx].head = new;
}

void addMailToToken(llist** table, char* key, int mail_id) {
    llist* dict = table[transfer[key[0]]];
    int idx = hashStr(key);
    node* cur_node = dict[idx].head;
    while (cur_node) {
        if (strcmp(key, cur_node->key) == 0) {
            cur_node->mail_cnt++;
            mlistInsert(cur_node->mail_list, mail_id);
            return;
        } else {
            cur_node = cur_node->nxt;
        }
    }
    newKey(table, key, mail_id);
}

mlist* getMails(llist** table, char* key) {
    llist* dict = table[transfer[key[0]]];
    int idx = hashStr(key);
    node* cur_node = dict[idx].head;
    while (cur_node) {
        if (strcmp(key, cur_node->key) == 0) {
            return cur_node->mail_list;
        } else {
            cur_node = cur_node->nxt;
        }
    }
    return NULL;
}

int getMailCnt(llist** table, char* key) {
    llist* dict = table[transfer[key[0]]];
    int idx = hashStr(key);
    node* cur_node = dict[idx].head;
    while (cur_node) {
        if (strcmp(key, cur_node->key) == 0) {
            return cur_node->mail_cnt;
        } else {
            cur_node = cur_node->nxt;
        }
    }
    return -1;
}

int main() {
    char k1[100] = "Hello!"; int v1 = 41;
    char k2[100] = "Hi!";    int v2 = 37;
    char k3[100] = "Howdy!"; int v3 = 29;
    char k4[100] = "Hola!";  int v4 = 13;
    char k5[100] = "Aloha!";
    llist** table = initTable();
    addMailToToken(table, k1, v1);
    addMailToToken(table, k1, v2);
    addMailToToken(table, k2, v2);
    addMailToToken(table, k3, v3);
    addMailToToken(table, k4, v4);
    printf("%d\n", getMailCnt(table, k1));
    printf("%d\n", getMailCnt(table, k2));
    printf("%d\n", getMailCnt(table, k3));
    printf("%d\n", getMailCnt(table, k4));
    printf("%d\n", getMailCnt(table, k5));
    return 0;
}