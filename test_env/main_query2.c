// By ChenKB
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include "api.h"

int n_mails, n_queries;
mail *mails;
query *queries;

// hash_table.h

#define MAGIC_MUL 8923 // 1361=>168 4583=>142 8923=>103
#define KEY_RANGE 10000019 // a large prime number
#define KEY_SIZE 55
#define MAIL_N 10005
#define GROUP_N 17

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

void addMailToToken(node* token_node, int mail_id, int* counter) {
    // Add a mail_id by token
    if (!token_node->mail_arr[mail_id]) {
        token_node->mail_cnt++;
        token_node->mail_arr[mail_id] = 1;
        mlistInsert(token_node->mail_list, mail_id);
    } else {
        *counter++;
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
            // COLLISION_CNT+=1;
            // printf("%s %s\n", key, cur_node->key);
            cur_node = cur_node->nxt;
        }
    }
    return newKey(&dict[idx], key);
}

// get_token.h

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
        if (s->content[i] <= 'Z' && s->content[i] >= 'A') s->content[i] += 32;
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
    st->sub_token = malloc(len*sizeof(char*)/2);
    st->sub_sz = 0;
    for (int i = 0; i <= len; i++) {
        if (s->subject[i] <= 'Z' && s->subject[i] >= 'A') s->subject[i] += 32;
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
    st->sub_token = realloc(st->sub_token, st->sub_sz*sizeof(char*));
    return;
}

// similar_table.h

typedef struct
{
    int *uniqToken; // number of unique tokens in each mail
    int **table;    // MailN x MailN array, intersecting tokens
} SimTable;

// Initialize a NxN int table
SimTable *initSimilar()
{
    SimTable *similar = malloc(sizeof(SimTable));
    similar->uniqToken = calloc(MAIL_N, sizeof(int));
    similar->table = calloc(100, sizeof(int *));
    for (int i = 0; i < 100; i++)
    {
        similar->table[i] = calloc(MAIL_N, sizeof(int));
    }
    return similar;
}
// add a series of tokens to the hashTable, but don't compute intersection yet
void addMailToHashtable(SimTable *sim, int mailID, str2token* st, llist **hashTable){
    int repeat = 0;
    int prv_repeat = 0;
    for (int i = 0; i < st->sz; i++)
    {
        char *key = st->token[i];
        node *key_node = getTokenNode(hashTable, key);
        addMailToToken(key_node, mailID, &repeat);
        if (repeat > prv_repeat)
        { // if the inserted key is already in there
            prv_repeat = repeat;
        }
    }
    sim->uniqToken[mailID] = st->sz - repeat;
}
// Add a mail to the similar table, using the hashtable
// void addToSimilar(SimTable *sim, int mailID, str2token* st, llist **hashTable)
// {
//     int repeat = 0;
//     int prv_repeat = 0;
//     for (int i = 0; i < st->sz; i++)
//     {
//         char *key = st->token[i];
//         node *key_node = getTokenNode(hashTable, key);
//         addMailToToken(key_node, mailID, &repeat);
//         if (repeat > prv_repeat)
//         { // if the inserted key is already in there
//             prv_repeat = repeat;
//         }
//         // else
//         {
//             int cnt = key_node->mail_cnt; 
//             if (cnt != -1)  // if there's no other mail with this token
//             {
//                 mnode *mail = key_node->mail_list->head; // getMails(hashTable, key, key_hash)->head;
//                 for (; mail != NULL; mail = mail->nxt)
//                 {
//                     sim->table[mailID][mail->mail_id] += 1;
//                     sim->table[mail->mail_id][mailID] += 1;
//                 }
//             }
//         }
//     }
//     sim->uniqToken[mailID] = st->sz - repeat;
// }
int findRowSimilar(SimTable *sim, llist **hashTable, int id, str2token *st, double thres, int* ansArr){
    int visited[KEY_RANGE] = {0};
    for( int i=0; i<st->sz; i++)
    {
        char *key = st->token[i];
        int hash = hashStr(key);
        if( !visited[hash] )
        {
            visited[ hashStr(key) ] = 1;
            node *key_node = getTokenNode(hashTable, key); 
            mnode *mail = key_node->mail_list->head; 
            for (; mail != NULL; mail = mail->nxt)
            {
                sim->table[id][mail->mail_id] += 1;
            }
        }
    }
    int len = 0;
    for( int i=0; i<n_mails; i++ )
    {
        if(sim->table[id][i] / (double)(sim->uniqToken[id] + sim->uniqToken[i]- sim->table[id][i]) > thres)
        {
            ansArr[len] = i;
            len += 1;
        }
    }
    return len;

}

// get similarity from a fully built table
double getSimilarity(SimTable *sim, int id1, int id2)
{
    int inter = sim->table[id1][id2];
    int onion = sim->uniqToken[id1] + sim->uniqToken[id2] - inter; // union is a keyword Q_Q
    return inter / (double)onion;
}

int main(){
    api.init(&n_mails, &n_queries, &mails, &queries);

    llist** hashtable = initTable();
    SimTable* simtable = initSimilar();
    str2token token_sets[n_mails];
    int ans_array[n_mails];

    for( int i=0; i<n_mails; i++){
        str2token* st = &token_sets[i];
        get_token(st, &mails[i]);
        addMailToHashtable(simtable, i, st, hashtable);
    }

    for(int i=0; i<n_queries; i++){
        int len = 0;
        if(queries[i].type == find_similar){
            int mid = queries[i].data.find_similar_data.mid;
            if( mid < 100 )
            {
                double thres = queries[i].data.find_similar_data.threshold;
                findRowSimilar(simtable, hashtable, mid, &token_sets[mid], thres, ans_array);
                api.answer(i,ans_array,len);

            }
            
        }
    }
}