#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "get_token.h"

#define MAGIC_MUL 37
#define KEY_RANGE 300007 // a large prime number
#define KEY_SIZE 55
#define MAIL_N 10005
#define GROUP_N 16

/*
Prime number source: https://primes.utm.edu/lists/small/100000.txt
*/

// Get index of the hash table by char
const int transfer[128] = { // 16 per line
    -1, -1, -1, -1, -1, -1, -1, -1,  -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1,  -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1,  -1, -1, -1, -1, -1, -1, -1, -1,
     0,  2,  1,  0,  0,  0,  0,  0,   0,  0, -1, -1, -1, -1, -1, -1,
    -1, 14,  8, 10,  3,  2,  6,  1,   5, 11,  0,  0,  3,  4,  1, 12,
     7,  0,  4, 13, 15,  1,  0,  9,   0,  0,  0, -1, -1, -1, -1, -1,
    -1, 14,  8, 10,  3,  2,  6,  1,   5, 11,  0,  0,  3,  4,  1, 12,
     7,  0,  4, 13, 15,  1,  0,  9,   0,  0,  0, -1, -1, -1, -1, -1
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
    // There seems to be some problem using calloc (Not Sure)
    // llist* dict = calloc(KEY_RANGE, sizeof(llist));
    llist* dict = malloc(KEY_RANGE*sizeof(llist));
    for (int i=0; i<KEY_RANGE; i++) {
        dict[i].head = NULL;
    }
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

void newKey(llist** table, char* key, int mail_id) {
    // Add new key
    llist* dict = table[transfer[key[0]]];
    node* new = malloc(sizeof(node));
    int idx = hashStr(key);
    strcpy(new->key, key);
    new->mail_cnt = 1;
    new->mail_list = newMlist();
    mlistInsert(new->mail_list, mail_id);
    memset(new->mail_arr, 0, MAIL_N);
    new->mail_arr[mail_id] = 1;
    new->nxt = dict[idx].head;
    dict[idx].head = new;
}

void addMailToToken(llist** table, char* key, int mail_id) {
    // Add a mail_id by token
    // Creates a new key if doesn't exist
    llist* dict = table[transfer[key[0]]];
    int idx = hashStr(key);
    // printf("belongs to dictionary %d\n", transfer[key[0]]);
    // printf("t dictionary %d\n", transfer['t']);
    node* cur_node = dict[idx].head;
    // printf("adding %s\n", key);
    while (cur_node) {
        if (strcmp(key, cur_node->key) == 0) {
            if (!cur_node->mail_arr[mail_id]) {
                cur_node->mail_cnt++;
                cur_node->mail_arr[mail_id] = 1;
                mlistInsert(cur_node->mail_list, mail_id);
            }
            return;
        } else {
            cur_node = cur_node->nxt;
            COLLISION_CNT++;
        }
    }
    newKey(table, key, mail_id);
}

mlist* getMails(llist** table, char* key) {
    // Get linked list of mails
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

int* getMailArr(llist** table, char* key) {
    // Get array of mails
    llist* dict = table[transfer[key[0]]];
    int idx = hashStr(key);
    node* cur_node = dict[idx].head;
    while (cur_node) {
        if (strcmp(key, cur_node->key) == 0) {
            return cur_node->mail_arr;
        } else {
            cur_node = cur_node->nxt;
        }
    }
    return NULL;
}

int getMailCnt(llist** table, char* key) {
    // Get number of mails
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
        // printf("===start===\n");
        // printf("%s\n", key);
        addMailToToken(hashTable, key, mailID);
        // printf("=== end ===\n");
        if (COLLISION_CNT > prev_collision){ // if the inserted key is already in there
            prev_collision = COLLISION_CNT;
        }else{
            int cnt = getMailCnt(hashTable, key);
            if(cnt!=-1){
                mnode* mail = getMails(hashTable, key)->head;
                for( ;mail!=NULL;mail=mail->nxt ){
                    sim->table[mailID][mail->mail_id] += 1;
                    sim->table[mail->mail_id][mailID] += 1;
                }
            }
        }
    }
    sim->uniqToken[mailID] = st->sz - COLLISION_CNT;
    // printf("Howdy_end!\n");
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


    str2token *st1, *st2, *st3, *st4;
    st1 = readTokenFromFile("data/pure/mail1");
    st2 = readTokenFromFile("data/pure/mail2");
    st3 = readTokenFromFile("data/pure/mail3");
    st4 = readTokenFromFile("data/pure/mail1");
    // printf("Howdy_main!\n");
    addToSimilar(sim, 0, st1, hashtable);
    addToSimilar(sim, 1, st2, hashtable);
    addToSimilar(sim, 2, st3, hashtable);
    addToSimilar(sim, 3, st4, hashtable);

    printf("%f\n", getSimilarity(sim, 0, 1));
    printf("%f\n", getSimilarity(sim, 0, 2));
    printf("%f\n", getSimilarity(sim, 0, 3));

    // */
    // For testing, k5 is intented to be a key not inserted
    //char k1[100] = "Hello!"; int v1 = 41;
    //char k2[100] = "Hi!";    int v2 = 37;
    //char k3[100] = "Howdy!"; int v3 = 29;
    //char k4[100] = "Hola!";  int v4 = 13;
    //char k5[100] = "Aloha!";
    //llist** table = initTable();
    //addMailToToken(table, k1, v1);
    //addMailToToken(table, k1, v2);
    //addMailToToken(table, k2, v2);
    //addMailToToken(table, k3, v3);
    //addMailToToken(table, k4, v4);
    //printf("getMailCnt():\n");
    //printf("%d\n", getMailCnt(table, k1));
    //printf("%d\n", getMailCnt(table, k2));
    //printf("%d\n", getMailCnt(table, k3));
    //printf("%d\n", getMailCnt(table, k4));
    //printf("%d\n", getMailCnt(table, k5));
    //printf("getMailArr():\n");
    //printf("%d\n", getMailArr(table, k1)[v1]);
    //printf("%d\n", getMailArr(table, k1)[v2]);
    //printf("%d\n", getMailArr(table, k1)[v3]);
    //printf("%d\n", getMailArr(table, k1)[v4]);

    return 0;
}
