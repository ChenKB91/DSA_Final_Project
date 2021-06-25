#define LAZY_NUM 3000
#define GROUP_MODL 20118
#define POW_BASE 31

int n_mails, n_queries;
mail *mails;
query *queries;

int gl[2];
int ans_arr[10005];

typedef struct {
    int parent, sz;
} dsu_unit;
dsu_unit dsu[GROUP_MODL];
int dsu_fill[GROUP_MODL];
void swap(int *a, int *b) {
    int _ = *a;
    *a = *b;
    *b = _;
}
int str_hash(char *a) {
    int ret = 0, now = 0;
    while (a[now]) {
        ret = (ret * POW_BASE + a[now]) % GROUP_MODL;
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
int set_union(int x, int y) {
    int set_x = find_set(x), set_y = find_set(y);
    if (dsu[set_x].sz < dsu[set_y].sz) 
        swap(&set_x, &set_y);
    if (set_x != set_y) {
        dsu[set_y].parent = set_x;
        dsu[set_x].sz += dsu[set_y].sz;
    }
    return dsu[set_x].sz;
}
void group_analyse_func(int n, mail* data, int *usr) {
    int ht[2];
    gl[0] = 0, gl[1] = 1;
    for (int i = 0; i < n; i++) {
        ht[0] = str_hash(data[usr[i]].from);
        ht[1] = str_hash(data[usr[i]].to);
        for (int j = 0; j < 2; j++) {
            if (!dsu_fill[ht[j]]) {
                dsu_fill[ht[j]] = 1;
                gl[0]++;
                dsu_init(ht[j]);
                if (ht[0] == ht[1]) gl[0]--;
            }
        }
        if (find_set(ht[0]) != find_set(ht[1])) {
            gl[0]--;
            int l = set_union(ht[0], ht[1]);
            gl[1] = gl[1] > l ? gl[1] : l;
        }
    }
}

int main() {
    api.init(&n_mails, &n_queries, &mails, &queries);
	for (int i = 0; i < n_queries; i++) {
		if (queries[i].type == find_similar) {
            int mid = queries[i].data.find_similar_data.mid;
            if (mid < LAZY_NUM) {
                double thres = queries[i].data.find_similar_data.threshold;
                int ans_len=0;
                for (int j=0; j<n_mails; j++) {
                    if (j==mid) continue;
                    if ( intersect[mid][j] / (double)(token_cnt[mid] + token_cnt[j] - intersect[mid][j]) > thres) {
                        ans_arr[ans_len++] = j;
                    }
                }
                api.answer(queries[i].id, ans_arr, ans_len);
            }
		}
	}
    for (int i = 0; i < n_queries; i++) {
        if (queries[i].type == group_analyse) {
            if (queries[i].reward >= 0.2) {
                memset(dsu_fill, 0, sizeof(dsu_fill));
                int len = queries[i].data.group_analyse_data.len;
                int *mids = queries[i].data.group_analyse_data.mids;
                group_analyse_func(len, mails, mids);
                api.answer(queries[i].id, gl, 2);
            }
        }
    }
	return 0;
}