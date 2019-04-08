#include <stdio.h>
#include <time.h>
#include <string.h>

#include "hw1_c.h"

int main(){
	clock_t before;
	double time;
	int* arr;
	int* tmp;
	int n, k;
	int ans;

	scanf("%d %d", &n, &k);
	arr = calloc(n, sizeof(int));
	tmp = calloc(n, sizeof(int));

	for(int i = 0; i < n; i++){
		scanf("%d", &arr[i]);
	}

	printf("Result of deterministic selection with n=%d\n", n);

	for(int i = 0; i < 15; i++){
		memcpy(tmp, arr, n * sizeof(int));
		
		before = clock();
		ans = deterministic_select(tmp, n, k);
		time = (double)(clock() - before) / (CLOCKS_PER_SEC/1000);
		if(i >= 5){
			printf("test %d\t: ", i - 4);
			if(checker(arr, n, k, ans)){
				printf("Correct!\t");
			}
		
			else {
				printf("Wrong!\t");
			}
			printf("Total time : %5.2fms\n", time);
		}
	}

	
	return 0;
}
	
