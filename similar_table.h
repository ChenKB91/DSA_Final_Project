// By ChenKB
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "get_token.h"
#include "hash_table.h"
#include "api.h"

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
    similar->table = calloc(MAIL_N, sizeof(int *));
    for (int i = 0; i < MAIL_N; i++)
    {
        similar->table[i] = calloc(MAIL_N, sizeof(int));
    }
    return similar;
}
// Add a mail to the similar table, using the hashtable
void addToSimilar(SimTable *sim, int mailID, mail* m, str2token* st, llist **hashTable)
{

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
        // else
        {
            int cnt = key_node->mail_cnt; 
            if (cnt != -1)  // if there's no other mail with this token
            {
                mnode *mail = key_node->mail_list->head; // getMails(hashTable, key, key_hash)->head;
                for (; mail != NULL; mail = mail->nxt)
                {
                    sim->table[mailID][mail->mail_id] += 1;
                    sim->table[mail->mail_id][mailID] += 1;
                }
            }
        }
    }
    sim->uniqToken[mailID] = st->sz - repeat;
}

double getSimilarity(SimTable *sim, int id1, int id2)
{
    int inter = sim->table[id1][id2];
    int onion = sim->uniqToken[id1] + sim->uniqToken[id2] - inter; // union is a keyword Q_Q
    return inter / (double)onion;
}

// int main()
// {
//     // /*
//     llist **hashtable = initTable();
//     SimTable *sim = initSimilar();
//     char buf[30];
//     for (int id = 1; id <= 10000; id++)
//     {
//         str2token *st;
//         sprintf(buf, "data/pure/mail%d", id);
//         st = readTokenFromFile(buf);
//         // addToSimilar(sim, id-1, st, hashtable);
//         for (int t = 0; t < st->sz; t++)
//         {
//             node *key_node = getTokenNode(hashtable, st->token[t]);
//             addMailToToken(key_node, id);
//         }
//     }
//     /* printf("%f\n", getSimilarity(sim, 0, 99)); */
//     /* printf("%f\n", getSimilarity(sim, 44, 55)); */
//     /* printf("%f\n", getSimilarity(sim, 22, 33)); */
//     /*  */
//     // */
//     char k5[100] = "the";
//     printf("%d\n", getTokenNode(hashtable, k5)->mail_cnt);
//     printf("Multiply: %d\n", MAGIC_MUL);
//     printf("Modulo: %d\n", KEY_RANGE);
//     // printf("Collision: %d\n", COLLISION_CNT);

//     return 0;
// }
