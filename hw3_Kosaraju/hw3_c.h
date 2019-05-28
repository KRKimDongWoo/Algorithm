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

void dfs_list_init(int root, int** adj, int* cnt);
void dfs_list_done(int root, int** adj, int* cnt, int gid);

void dfs_mat_init(int root, int** adj, int num_v);
void dfs_mat_done(int root, int** adj, int num_v, int gid);

int* stack;
int top;

int* visited;
int* group;

int find_scc_with_adj_list(int **adj,int *cnt,int num_v,int num_e,int *ans){

	int** rev_adj = (int**) calloc(num_v, sizeof(int*));
	int* rev_cnt = (int*) calloc(num_v, sizeof(int));

	for(int i = 0; i < num_v; i++) {
		for(int j = 0; j < cnt[i]; j++) {
			int dest = adj[i][j];
			rev_adj[dest] = realloc(rev_adj[dest], (++rev_cnt[dest]) * sizeof(int));
			rev_adj[dest][rev_cnt[dest]-1] = i;
		}
	}

	int* group_translate = (int*) calloc(num_v, sizeof(int));
	int scc_num = 0;

	stack = (int*) calloc(num_v, sizeof(int));
	top = 0;

	visited = (int*) calloc(num_v, sizeof(int));
	group = (int*) calloc(num_v, sizeof(int));

	for(int i = 0; i < num_v; i++) {
		if(!visited[i])
			dfs_list_init(i, adj, cnt);
	}

	for(int i = num_v - 1; i >= 0; i--) {
		int node = stack[i];
		if(visited[node]) {
			//printf("\n%d can be a head of scc...\n", node);
			dfs_list_done(node, rev_adj, rev_cnt, node);
		}
	}
	for(int i = 0; i < num_v; i++) {
		if(!group_translate[group[i] - 1])
			group_translate[group[i] - 1] = ++scc_num;
		ans[i] = group_translate[group[i] - 1] - 1;
	}

	free(stack);
	free(visited);
	free(group);

	return scc_num;
}

void dfs_list_init(int root, int** adj, int* cnt) {
	visited[root] = TRUE;

	for(int i = 0; i < cnt[root]; i++) {
		int node = adj[root][i];
		if(!visited[node])
			dfs_list_init(node, adj, cnt);
	}
	stack[top++] = root;
}

void dfs_list_done(int root, int** adj, int* cnt, int gid) {
	visited[root] = FALSE;
	group[root] = gid;

	//printf("%d ", root);

	for(int i = 0; i < cnt[root]; i++) {
		int node = adj[root][i];
		if(visited[node])
			dfs_list_done(node, adj, cnt, gid);
	}
}



//adj : num_v x num_v dimension adjacency matrix of the given graph

//the other parameters have same as above function.

int find_scc_with_adj_mat(int **adj,int num_v,int num_e,int *ans){

	int* group_translate = (int*) calloc(num_v, sizeof(int));
	int scc_num = 0;

	stack = (int*) calloc(num_v, sizeof(int));
	top = 0;

	visited = (int*) calloc(num_v, sizeof(int));
	group = (int*) calloc(num_v, sizeof(int));

	for(int i = 0; i < num_v; i++) {
		if(!visited[i])
			dfs_mat_init(i, adj, num_v);
	}

	for(int i = num_v - 1; i >= 0; i--) {
		int node = stack[i];
		if(visited[node]) {
			//printf("\n%d can be a head of scc...\n", node);
			dfs_mat_done(node, adj, num_v, node);
		}
	}
	for(int i = 0; i < num_v; i++) {
		if(!group_translate[group[i] - 1])
			group_translate[group[i] - 1] = ++scc_num;
		ans[i] = group_translate[group[i] - 1] - 1;
	}

	free(stack);
	free(visited);
	free(group);

	return scc_num;
}

void dfs_mat_init(int root, int** adj, int num_v) {
	visited[root] = TRUE;

	for(int i = 0; i < num_v; i++) {
		if(!adj[root][i]) continue;
		if(!visited[i]) dfs_mat_init(i, adj, num_v);
	}

	stack[top++] = root;
}

void dfs_mat_done(int root, int** adj, int num_v, int gid) {
	visited[root] = FALSE;
	group[root] = gid;

	for(int i = 0; i < num_v; i++) {
		if(!adj[i][root]) continue;
		if(visited[i]) dfs_mat_done(i, adj, num_v, gid);
	}
}

#endif
