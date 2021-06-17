#include <stdio.h>

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

node* getTokenNode(llist** table, char* key);

int evalSingle(char** expr_ptr, int mail_id, llist** hashtable) {
    int and_pc = 1;
    int or_pc = 0;
    int tk_len = 0;
    char tk[55];
    while (**expr_ptr) {
        switch (**expr_ptr) {
            case '(':
                *expr_ptr++;
                and_pc = (and_pc || evalSingle(expr_ptr, mail_id, hashtable));
                break;
            case '!':
                *expr_ptr++;
                return (! evalSingle(expr_ptr, mail_id, hashtable));
            case '&':
                if (!and_pc) {
                    // lazy evaluation
                } else {
                    
                }
                break;
            case '|':

                break;
            default:
                while (**expr_ptr != ')') {
                    tk[tk_len] = **expr_ptr;
                    tk_len++;
                    *expr_ptr++;
                }
                tk[tk_len] = '\0';
                *expr_ptr++;
                node* tk_node = getTokenNode(hashtable, tk);
                return tk_node->mail_arr[mail_id];
            
        }
    }
}

int main(){
    
    return 0;
}