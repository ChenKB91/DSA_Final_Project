#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAGIC_MUL 31
#define MAGIC_REM 1000000007
#define KEY_SIZE 10000

typedef struct __node__ {
    char key[KEY_SIZE];
    int val;
    struct __node__* nxt;
} node;

typedef struct __llist__ {
    node* head;
} llist;


int hashStr(char* s) {
    long long int hash = 0;
    while (*s) {
        hash = (MAGIC_MUL*hash+(*s++)) % MAGIC_REM;
    }
    return hash;
}

llist* newDict() {
    llist* dict = malloc(MAGIC_REM*sizeof(llist));
    for (int i=0; i<MAGIC_REM; i++) {
        dict[i].head = NULL;
    }
    return dict;
}

void addEntry(llist* dict, char* key, int val) {
    node* new = malloc(sizeof(node));
    int idx = hashStr(key);
    strcpy(new->key, key);
    new->val = val;
    new->nxt = dict[idx].head;
    dict[idx].head = new;
}

int lookup(llist* dict, char* key) {
    // return -1 if not in dictionary
    int idx = hashStr(key);
    node* cur_node = dict[idx].head;
    while (cur_node) {
        if (strcmp(key, cur_node->key) == 0) {
            return cur_node->val;
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
    llist* dict = newDict();
    addEntry(dict, k1, v1);
    addEntry(dict, k2, v2);
    addEntry(dict, k3, v3);
    addEntry(dict, k4, v4);
    printf("%d\n", lookup(dict, k1));
    printf("%d\n", lookup(dict, k2));
    printf("%d\n", lookup(dict, k3));
    printf("%d\n", lookup(dict, k4));
    
    return 0;
}