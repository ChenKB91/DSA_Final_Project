#define LAZY_NUM 1000
int n_mails, n_queries;
mail *mails;
query *queries;

int ans_arr[10005];

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
	return 0;
}