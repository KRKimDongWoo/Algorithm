#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "hw3_c.h"
#define RUNNING_REP 100

#ifndef FILE_INPUT_ENABLED
#define STD_INPUT_ENABLED
#endif

int main(){
	int num_v;
	int num_e;
	
	int *cnt;
	int **adj_list;
	int **adj_mat;
	
	clock_t start_list, start_mat, end_list, end_mat;

	int from, dest;
#if defined(STD_INPUT_ENABLED)
	printf("Standard input mode\n");
	scanf("%d %d", &num_v, &num_e);
#elif defined(FILE_INPUT_ENABLED)
#ifndef INPUT_FILE
#define INPUT_FILE ""
#endif
	printf("File input mode\n");
	FILE *fp = fopen(INPUT_FILE, "r");
	if(fp == NULL) {
		printf("Invalid input file: %s\n", INPUT_FILE);
		exit(1);
	}
	printf("Testing with %s\n", INPUT_FILE);
	fscanf(fp, "%d %d", &num_v, &num_e);
#endif

	adj_list = (int **) calloc(num_v, sizeof(int*));
	adj_mat = (int **) calloc(num_v, sizeof(int*));
	cnt = (int*) calloc(num_v, sizeof(int));

	for(int i = 0; i < num_v; i++) {
		adj_list[i] = NULL;
		adj_mat[i] = (int *) calloc(num_v, sizeof(int));
		cnt[i] = 0;
	}

	for(int i = 0; i < num_e; i++) {
#if defined(STD_INPUT_ENABLED)
		scanf("%d %d", &from, &dest);
#elif defined(FILE_INPUT_ENABLED)
		fscanf(fp, "%d %d", &from, &dest);
#endif
		adj_list[from] = (int *) realloc(adj_list[from], (++cnt[from]) * sizeof(int));
		adj_list[from][cnt[from] - 1] = dest;

		adj_mat[from][dest] = 1;
	}

	int* ans_list = (int*) calloc(num_v, sizeof(int));
	int* ans_mat = (int*) calloc(num_v, sizeof(int));
	
	start_list = clock();
	for(int i = 0; i < RUNNING_REP; i++)
		find_scc_with_adj_list(adj_list, cnt, num_v, num_e, ans_list);
	end_list = clock();	
	
	start_mat = clock();
	for(int i = 0; i < RUNNING_REP; i++)
		find_scc_with_adj_mat(adj_mat, num_v, num_e, ans_mat);
	end_mat = clock();



#if defined(STD_INPUT_ENABLED)
	printf("Result on list\n");
	for(int i = 0; i < num_v; i++)
		printf("%d ", ans_list[i]);
	printf("\nResult on matrix\n");
	for(int i = 0; i < num_v; i++)
		printf("%d ", ans_mat[i]);
	printf("\n");
#elif defined(FILE_INPUT_ENABLED)
#ifndef OUTPUT_FILE
#define OUTPUT_FILE ""
#endif
	fp = fopen(OUTPUT_FILE, "r");
	int* ans = (int*) calloc(num_v, sizeof(int));
	int err_flag = 0;

	printf("Checking list...\n");
	for(int i = 0; i < num_v; i++) {
		fscanf(fp, "%d", &ans[i]);
	}
	for(int i = 0; i < num_v; i++){
		if(ans[i] != ans_list[i]) {
			err_flag = 1;
			printf("Error on list.\n");
			printf("On line %d, expected: %d, got: %d\n", i, ans[i], ans_list[i]);
			break;
		}
	}

	if(!err_flag) printf("Successful!\n");
	err_flag = 0;	
	
	printf("Checking matrix...\n");
	for(int i = 0; i < num_v; i++){
		if(ans[i] != ans_mat[i]) {
			err_flag = 1;
			printf("Error on mat\n");
			printf("On line %d, expected: %d, got: %d\n", i, ans[i], ans_list[i]);
			break;
		}
	}

	if(!err_flag) printf("Successful!\n");
#endif

	printf("list takes %0.3fms\n", (float)(end_list - start_list)/(CLOCKS_PER_SEC / 1000)/RUNNING_REP);
	printf("mat takes %0.3fms\n", (float)(end_mat - start_mat)/(CLOCKS_PER_SEC / 1000)/RUNNING_REP);
	return 0;
}
