#include "api.h"
#include <stdio.h>

// The testdata only contains the first 100 mails (mail1 ~ mail100)
// and 2000 queries for you to debug.

int n_mails, n_queries;
mail *mails;
query *queries;

int main(void){
	api.init(&n_mails, &n_queries, &mails, &queries);
	int cnt = 0;
	double reward = 0;
	for (int i = 0; i < n_queries; i++) {
		if (queries[i].type == group_analyse) {
			reward += queries[i].reward;
			cnt++;
		}
		// 	printf("%d\n", i);
		// 	// api.answer(queries[i].id, NULL, 0);
	}
	printf("%d\n", cnt);
	printf("%f\n", reward);
	return 0;
}
