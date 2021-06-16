#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "get_token.h"

#define MAGIC_MUL 149
#define KEY_RANGE 1000003 // a large prime number
#define KEY_SIZE 55
#define MAIL_N 10005
#define GROUP_N 17

/*
Prime number source: https://primes.utm.edu/lists/small/100000.txt
*/

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

const int toInt[128] = { // 16 per line
    -1, -1, -1, -1, -1, -1, -1, -1,  -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1,  -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1,  -1, -1, -1, -1, -1, -1, -1, -1,
    27, 28, 29, 30, 31, 32, 33, 34,  35, 36, -1, -1, -1, -1, -1, -1,
    -1,  1,  2,  3,  4,  5,  6,  7,   8,  9, 10, 11, 12, 13, 14, 15,
    16, 17, 18, 19, 20, 21, 22, 23,  24, 25, 26, -1, -1, -1, -1, -1,
    -1,  1,  2,  3,  4,  5,  6,  7,   8,  9, 10, 11, 12, 13, 14, 15,
    16, 17, 18, 19, 20, 21, 22, 23,  24, 25, 26, -1, -1, -1, -1, -1
};

// Count collision times during addMailToToken()
int COLLISION_CNT = 0;

int hashStr(char* s) {
    long long int hash = 0;
    while (*s) {
        hash = (MAGIC_MUL*hash+(*s++)) % KEY_RANGE;
        // hash = (37*hash+(toInt[*s++])) % KEY_RANGE;
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

typedef struct{
    int* uniqToken; // number of unique tokens in each mail
    int** table;  // MailN x MailN array, intersecting tokens
}SimTable;

SimTable* initSimilar(){
    SimTable* similar = malloc(sizeof(SimTable));
    similar->uniqToken = calloc(MAIL_N,sizeof(int));
    similar->table = calloc(MAIL_N,sizeof(int*));
    for(int i=0;i<MAIL_N;i++){
        similar->table[i] = calloc(MAIL_N,sizeof(int));
    }
    return similar;
}
// Add a mail to the similar table, using the hashtable
void addToSimilar(SimTable* sim, int mailID, str2token* st, llist** hashTable){
    COLLISION_CNT = 0;
    int prev_collision = COLLISION_CNT;
    for(int i=0;i<st->sz;i++){
        char *key = st->token[i];
        node* key_node = getTokenNode(hashTable, key);
        addMailToToken(key_node, mailID);
        if (COLLISION_CNT > prev_collision){ // if the inserted key is already in there
            prev_collision = COLLISION_CNT;
        }else{
            int cnt = key_node->mail_cnt; // getMailCnt(hashTable, key, key_hash);
            if(cnt!=-1){
                mnode* mail = key_node->mail_list->head; // getMails(hashTable, key, key_hash)->head;
                for( ;mail!=NULL;mail=mail->nxt ){
                    sim->table[mailID][mail->mail_id] += 1;
                    sim->table[mail->mail_id][mailID] += 1;
                }
            }
        }
    }
    sim->uniqToken[mailID] = st->sz - COLLISION_CNT;
}

double getSimilarity(SimTable* sim, int id1, int id2){
    int inter = sim->table[id1][id2];
    int onion = sim->uniqToken[id1] + sim->uniqToken[id2] - inter; // union is a keyword Q_Q
    return inter/(double)onion;
}

int main() {
    // /*
    llist** hashtable = initTable();
    SimTable* sim = initSimilar();
    char buf[30];
    for(int id=1; id<=10000; id++){
        str2token *st;
        sprintf(buf,"data/pure/mail%d",id);
        st = readTokenFromFile(buf);
        // addToSimilar(sim, id-1, st, hashtable);
        for(int t=0;t<st->sz;t++){
            node* key_node = getTokenNode(hashtable, st->token[t]);
            addMailToToken(key_node, id);
        }
    }
    /* printf("%f\n", getSimilarity(sim, 0, 99)); */
    /* printf("%f\n", getSimilarity(sim, 44, 55)); */
    /* printf("%f\n", getSimilarity(sim, 22, 33)); */
    /*  */
    // */
    char k5[100] = "the";
    printf("%d\n", getTokenNode(hashtable, k5)->mail_cnt);
    printf("Multiply: %d\n", MAGIC_MUL);
    printf("Modulo: %d\n", KEY_RANGE);
    printf("Collision: %d\n", COLLISION_CNT);

    return 0;
}
