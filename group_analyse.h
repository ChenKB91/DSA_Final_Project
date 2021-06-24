// Code by hyder308116
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
const int name_hash_mod = 20118;
typedef struct {
    int parent, sz;
} dsu_unit;
dsu_unit dsu[20118];
int dsu_fill[20118];
void swap(int *a, int *b) {
    int _ = *a;
    *a = *b;
    *b = _;
}
int str_hash(char *a) {
    int ret = 0, now = 0;
    while (a[now]) {
        ret = (ret * 31 + a[now]) % name_hash_mod;
        now++;
    }
    return ret;
}
void dsu_init(int n) {
    dsu[n].parent = n;
    dsu[n].sz = 1;
}
int find_set(int x) {
    if (dsu[x].parent == x) return x;
    dsu[x].parent = find_set(dsu[x].parent);
    return dsu[x].parent;
}
void set_union(int x, int y) {
    int set_x = find_set(x), set_y = find_set(y);
    if(dsu[set_x].sz < dsu[set_y].sz)
        swap(&set_x, &set_y);
    if (set_x != set_y) {
        dsu[set_y].parent = set_x;
        dsu[set_x].sz += dsu[set_y].sz;
    }
    return dsu[set_x].sz;
}
int* group_analyse(int n, mail* data, int *usr) {
    // n: the amount of users
    // data: mail information
    // usr: index of mail in this analyse
    int gl[2] = {0}, ht[2];
    for (int i = 0; i < n; i++) {
        ht[0] = str_hash(mail[usr[i]]->from);
        ht[1] = str_hash(mail[usr[i]]->to);
        for (int j = 0; j < 2; j++) {
            if (!dsu_fill[ht[j]]) {
                dsu_fill[ht[j]] = 1;
                gl[0]++;
                dsu_init(ht[j]);
            }
        }
        if (find_set(ht[0]) != find_set(ht[1])) {
            gl[0]--;
            int l = set_union(ht[0], ht[1]);
            gl[1] = gl[1] > l ? gl[1] : l;
        }
    }
    return gl;
}
