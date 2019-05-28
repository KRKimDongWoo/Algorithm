#ifndef HW3C_H
#define HW3C_H

#include <stdlib.h>
#include <stdio.h>

//parameters
//adj : adjacent list of graph where the smallest element is the 0th element.
	//in array adj[i], neighbors of the ith vertecies are inserted.
//cnt : the number of neighbors of the ith vertex.
//num_v : the number of vertecies.
//num_e : the number of edges.
//ans : ans[i] means that ith vertex is included in the ans[i]th strongly connected component.
	//ans[i] can be non-negative integer.
	//if there are multiple answers, please find the lexicoraphically smallest sequence.

//This function should return the number of strongly connnected components.

#define min(x, y) ((x < y) ? x : y)
#define UNVISITED (0)
#define FALSE (0)
#define TRUE (1)

void dfs_list(int root, int** adj, int* cnt);
void dfs_mat(int root, int** adj, int num_v);

static int* ids;
static int* low;
static int* onStack;

static int* stack;
static int top;

static int id;

int find_scc_with_adj_list(int **adj,int *cnt,int num_v,int num_e,int *ans){
	ids = (int *) calloc(num_v, sizeof(int));
	low = (int *) calloc(num_v, sizeof(int));
	onStack = (int *) calloc(num_v, sizeof(int));

	stack = (int *) calloc(num_v, sizeof(int));
	top = 0;

	id = 1;

	int* low_translate = calloc(num_v, sizeof(int));
	int scc_num = 0;

	for(int i = 0; i < num_v; i++) {
		if(!ids[i]) {
			dfs_list(i, adj, cnt);
		}
		low_translate[i] = -1;
	}

	for(int i = 0; i < num_v; i++) {
		int original_low = low[i];
		if(low_translate[original_low - 1] == -1)
			low_translate[original_low - 1] = scc_num++;
		ans[i] = low_translate[original_low - 1];
	}

	free(ids);
	free(low);
	free(onStack);
	free(stack);

	return scc_num;
}

void dfs_list(int root, int** adj, int* cnt) {
	stack[top++] = root;
	onStack[root] = TRUE;
	ids[root] = (low[root] = id++);

	for(int i = 0; i < cnt[root]; i++) {
		int node = adj[root][i];
		if(!ids[node]) {
			dfs_list(node, adj, cnt);
		}
		if(onStack[node]) {
			low[root] = min(low[node], low[root]);
		}
	}

	if(ids[root] == low[root]) {
		for(int node = stack[--top];; node = stack[--top]) {
			onStack[node] = FALSE;
			low[node] = ids[root];
			if(node == root) break;
		}
	}
}



//adj : num_v x num_v dimension adjacency matrix of the given graph

//the other parameters have same as above function.

int find_scc_with_adj_mat(int **adj,int num_v,int num_e,int *ans){

	ids = (int *) calloc(num_v, sizeof(int));
	low = (int *) calloc(num_v, sizeof(int));
	onStack = (int *) calloc(num_v, sizeof(int));

	stack = (int *) calloc(num_v, sizeof(int));
	top = 0;

	id = 1;

	int* low_translate = calloc(num_v, sizeof(int));
	int scc_num = 0;

	for(int i = 0; i < num_v; i++) {
		if(!ids[i]) {
			dfs_mat(i, adj, num_v);
		}
		low_translate[i] = -1;
	}

	for(int i = 0; i < num_v; i++) {
		int original_low = low[i];
		if(low_translate[original_low - 1] == -1)
			low_translate[original_low - 1] = scc_num++;
		ans[i] = low_translate[original_low - 1];
	}

	free(ids);
	free(low);
	free(onStack);
	free(stack);

	return scc_num;
}

void dfs_mat(int root, int** adj, int num_v) {

	stack[top++] = root;
	onStack[root] = TRUE;
	ids[root] = (low[root] = id++);

	for(int node = 0; node < num_v; node++) {
		if(!adj[root][node]) continue;
		else {
			if(!ids[node]) {
				dfs_mat(node, adj, num_v);
			}
			if(onStack[node]) {
				low[root] = min(low[node], low[root]);
			}
		}
	}

	if(ids[root] == low[root]) {
		for(int node = stack[--top];; node = stack[--top]) {
			onStack[node] = FALSE;
			low[node] = ids[root];
			if(node == root) break;
		}
	}
}

#endif
