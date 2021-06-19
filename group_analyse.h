#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
typedef struct {
    char _name[20];
    int parent, sz;
} dsu_unit;
dsu_unit dsu[1000007];
int dsu_fill[1000007] = {0};
void swap(int *a, int *b) {
    int _ = *a;
    *a = *b;
    *b = _;
}
int str_hash(char *a) {
    int ret = 0, now = 0;
    while(a[now]) {
        ret = (ret * 31 + a[now]) % 1000007;
        now++;
    }
    return ret;
}
void dsu_init(int n, char *usr) {
    dsu[n].parent = n;
    dsu[n].sz = 1;
    strcpy(usr, dsu[n]._name);
}
int find_set(int x) {
    if (dsu[x].parent==x) return x;
    dsu[x].parent = find_set(dsu[x].parent);
    return dsu[x].parent;
}
void set_union(int x, int y) {
    int set_x = find_set(x), set_y = find_set(y);
    if (set_x != set_y) {
        if (dsu[set_x].sz < dsu[set_y].sz)
            swap(&set_x, &set_y);
        dsu[set_y].parent = set_x;
        dsu[set_x].sz += dsu[set_y].sz;
        return dsu[set_x].sz;
    }
    return dsu[set_x].sz > dsu[set_y].sz ? dsu[set_x].sz : dsu[set_y].sz;
}
int* group_analyse(int n, char ***data) {
    int gl[2] = {0}, ht[2]; // gl[0]:n_g, gl[1]:l_g
    for (int i = 0; i < n; i++) {
        ht[0] = str_hash(data[i][0]);
        ht[1] = str_hash(data[i][1]);
        for (int j = 0; j < 2; j++) {
            if (dsu_fill[ht[j]]) {
                while (strcmp(data[i][j], dsu[ht[j]]._name))
                    ht[j]++
            }
            if (!dsu_fill[ht[j]]) {
                dsu_init(ht[j], data[i][j]);
                gl[0]++;
            }
        }
        if (find_set(dsu[ht[0]].parent) != find_set(dsu[ht[1]].parent)) gl[0]--;
        int l = set_union(ht[0], ht[1]);
        gl[1] = gl[1] > l ? gl[1] : l;
    }
    return gl;
}
