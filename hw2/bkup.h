#ifndef HW2
#define HW2

#include <stdlib.h>
#include <stdio.h>

/* enumerate colors */
typedef enum {
	RED,
	BLACK
} color_t;

/* enumerate direction of childs */
typedef enum {
	LEFT = 0,
	RIGHT = 1,
	NONE = -1
} direction_t;

/* make the node structure */
typedef struct node {
	int size; //size of subtree with root itself
	int data; //data of node
	struct node* parent;
	struct node* left;
	struct node* right;
	color_t color;
} node_t;

/* Safe get functions.
 * These functions also handle for NULL input.
 * get_color will return BLACK and get_size will return 0,
 * as NULL is also a node with black color.
 */
color_t get_color(node_t* T);
int get_size(node_t* T);

/* Family finding helper functions.
 * These functions will return pointer of sibling/grandparent/
 * uncle, no matter if they are left or right child.
 */
node_t* get_sibling(node_t* T);
node_t* get_grandparent(node_t* T);
node_t* get_uncle(node_t* T);
node_t* get_one_child(node_t* T);

/* Safe set functions.
 * These functions also handle for NULL inputs.
 * If trying to add some family to NULL node, it does nothing.
 */
void set_child(node_t*P, node_t* C, direction_t dir);
void set_right(node_t* P, node_t* C);
void set_left(node_t* P, node_t* C);
void set_parent(node_t* C, node_t* P);
void set_color(node_t* T, color_t c);

/* Safe size update function.
 * It will do nothing if input is NULL.
 * Otherwise, it will set the new size, by calculating
 * left_tree->size + right_tree->size + 1
 */
void update_size(node_t* T);

/* Location helper function.
 * Returns 1 if it is right child of parent,
 * Returns 0 if it is left child of parent,
 * Returns -1 if it does not have parent.
 */
direction_t which_child(node_t* T);
int is_right(node_t* T);

/* Rotate helper functions.
 * Input as a root of the subtree that will be rotated.
 * Returns the new root of the subtree.
 */
node_t* rotate(node_t* T, direction_t dir);
node_t* left_rotate(node_t* T);
node_t* right_rotate(node_t* T);

/* Stablize helper functions.
 * Input as a root of the subtree that have to be corrected.
 */
void insert_stablize(node_t* T);
void delete_stablize(node_t* T);

/* Helper function for print tree in 2D
 */
void print_tree(node_t* T, int space);

/* Global variables.
 */
int v[10000];
int cnt;
static node_t* root = NULL;


int cmp(const void *a, const void *b){
	int n1 = *(int*)a;
	int n2 = *(int*)b;
	if(n1 < n2){
		return -1;
	}
	else if(n1 > n2){
		return 1;
	}
	else{
		return 0;
	}
}

void init(){
	cnt = 0;
}

int os_insert(int x){
    for(int i=0;i<cnt;i++){
		if(x==v[i]){
			return 0;
		}
	}
	
	node_t* curr = root;
	node_t* new_node = (node_t*) malloc(sizeof(node_t));
	
	/* initialize a new node */
	new_node->size = 1;
	new_node->data = x;
	new_node->parent = NULL;
	new_node->left = NULL;
	new_node->right = NULL;
	new_node->color = RED;


	if(cnt == 0) {
		/* None element is current.
		 * Therefore, set a new Node as a root node. 
		 */
		set_color(new_node, BLACK);
		root = new_node;
	}
	else {
		while(1) {
			if(curr->data < x) {
				/* current node is smaller then x, move to right. */
				curr->size += 1;
				if(curr->right == NULL) {
					curr->right = new_node;
					new_node->parent = curr;
					break;
				}
				else {
					curr = curr->right;
				}
			}
			else if(curr->data > x) {
				/* current node is larger then x, move to left */
				curr->size += 1;
				if(curr->left == NULL) {
					curr->left = new_node;
					new_node->parent = curr;
					break;
				}
				else {
					curr = curr->left;
				}
			}
			else {
				/* Current node is same with x.
				 * Expecting never reach here. */
				return 0;
			}
		}

		insert_stablize(new_node);
	}

	v[cnt++] = x;
	print_tree(root, 0);
	return x;
}

int os_delete(int x){
	return 0;

    int k = 0;
	int flag = 0;
	node_t* curr = root;
	node_t* target;

	for(int i=0;i<cnt;i++){
		if(v[i]==x){
			k = 1;
		}
		else{
			v[i-k] = v[i];
		}
	}

	if(k==1){
		/* Found element : delete and return x */

		while(curr != NULL) {
			curr->size -= 1;
			if(x > curr->data)
				curr = curr->right;
			else if(x < curr->data)
				curr = curr->left;
			else
				break;
		}
		
		if((curr->left != NULL) && (curr->right != NULL)) {
			/* Current node has two children.
			 * Swap with the left-most node on right subtree.
			 * And then, changes the target.
			 */
			target = curr->left;
			while(target != NULL) {
				target->size -= 1;
				if(target->right != NULL)
					target = target->right;
				else
					break;
			}
			int tmp = target->data;
			target->data = curr->data;
			curr->data = tmp;
		}
		else
			target = curr;

		/* Remove the target */
		if((get_color(target) == BLACK) && (get_color(get_one_child(target)) == BLACK)){
			/* Target and its child are both black.
			 * Problem occured, so set flag.
			 */
			flag = 1;
		}

		node_t* P = target->parent;
		node_t* C = get_one_child(target);


		if(is_right(target) == 1) {
			set_right(P, C);
			set_parent(C, P);
		}
		else if(is_right(target) == 0) {
			set_left(P, C);
			set_parent(C, P);
		}
		else {
			root = C;
		}
		free(target);
		set_color(C, BLACK);

		if(flag){
			delete_stablize(P);
		}

		--cnt;
		print_tree(root, 0);
		return x;
	}

	else{
		/* No element : return 0 */
		return 0;
	}
}

int os_select(int i){
    if(i-1<cnt){
		qsort(v,cnt,sizeof(int),cmp);
        return v[i-1];
    }
    else{
        return 0;
    }   
}

int os_rank(int x){
	qsort(v,cnt,sizeof(int),cmp);
	for(int i=0;i<cnt;i++){
		if(v[i]==x){
			return i + 1;
		}
	}
	return 0;
}

int check(int opt_seq[], int in_seq[], int out_seq[], int n){
    init();
    for(int i=0;i<n;i++){
		if(opt_seq[i]==0){
            if(os_insert(in_seq[i])!=out_seq[i]){
                return 0;
            }
        }
        else if(opt_seq[i]==1){
            if(os_delete(in_seq[i])!=out_seq[i]){
                return 0;
            }
        }
        else if(opt_seq[i]==2){
            if(os_select(in_seq[i])!=out_seq[i]){
                return 0;
            }
        }
        else if(opt_seq[i]==3){
            if(os_rank(in_seq[i])!=out_seq[i]){
                return 0;
            }
        }
    }
    return 1;
}

void insert_stablize(node_t* T) {
	if(T == NULL) {
		/* Wrong input, nothing to do.
		 * Expecting never reach here. 
		 */
		return;
	}
	if(T->parent == NULL) {
		/* T is a root node.
		 * Changes color to black if it's red.
		 */
		if(T->color == RED)
			T->color = BLACK;
		return;
	}
	else {
		if(T->color == RED) {
			/* T is red.
			 * Check for correctness.
			 */
			if(T->parent->color == RED) {
				/* Parent is red.
				 * RB tree is collapsed, so stablize.
				 */
				if(get_color(get_uncle(T)) == RED){
					/* Case 1
					 * T is red, p is red and s is red.
					 * Change the color, and stablize recursively.
					 */
					set_color(T->parent, BLACK);
					set_color(get_uncle(T), BLACK);
					set_color(get_grandparent(T), RED);

					insert_stablize(get_grandparent(T));
					return;
				}
				else {
					/* Case 2
					 * T is red, p is red and s is black.
					 * Rotate according to current location.
					 */
					if(is_right(T) ^ is_right(T->parent)) {
						/* Case 2-1
						 * T and parent grown in different direction.
						 * Rotate and stablize recursively.
						 * It will call case 2-2 in result.
						 */
						node_t* G = get_grandparent(T);
						if(is_right(T)) {
							set_left(G, left_rotate(T->parent));
							insert_stablize(T->left);
							return;	
						}
						else {
							set_right(G, right_rotate(T->parent));
							insert_stablize(T->right);
							return;
						}
					}
					else {
						/* Case 2-2
						 * T and parent grow in same direction.
						 * Rotate and change color.
						 */
						node_t* G = get_grandparent(T);
						node_t* PG = G->parent;
						if(is_right(T)) {
							if(is_right(G) == 1)
								set_right(PG, left_rotate(G));
							else if(is_right(G) == 0)
								set_left(PG, left_rotate(G));
							else
								root = left_rotate(G);
							
							set_color(G, RED);
							set_color(G->parent, BLACK);
							
							return;
						}
						else {
							if(is_right(G) == 1)
								set_right(PG, right_rotate(G));
							else if(is_right(G) == 0)
								set_left(PG, right_rotate(G));
							else 
								root = right_rotate(G);
							
							set_color(G, RED);
							set_color(G->parent, BLACK);
							
							return;
						}
					}
				}
			}
		}
	}
}

void delete_stablize(node_t* T) {
	node_t* P = T->parent;
	node_t* S = get_sibling(T);
	node_t* L = S->left;
	node_t* R = S->right;
	if(is_right(T) == 1) {
		if(get_color(S) == BLACK) {
			if(L == RED) {
				/* case X-2 */
				node_t* G = P->parent;
				if(is_right(P) == 1) {
					set_right(G, right_rotate(P));
				}
				else if(is_right(P) == 0) {
					set_left(G, right_rotate(P));
				}
				else {
					root = right_rotate(P);
				}
				set_color(S, P->color);
				set_color(P, BLACK);
				set_color(L, BLACK);
				return;
			}
			else {
				if(get_color(R) == RED) {
					/* case X-3 */
					set_right(P, left_rotate(S));
					set_color(R, BLACK);
					set_color(S, RED);
					delete_stablize(T);
					return;
				}
				else {
					if(get_color(P) == RED) {
						/* case 1-1 */
						set_color(P, RED);
						set_color(S, BLACK);
						return;
					}
					else {
						/* case 2-1 */
						set_color(S, RED);
						delete_stablize(P);
						return;
					}
				}
			}
		}
		else {
			/* Case 2-4 */
			node_t* G = P->parent;
			if(is_right(P) == 1) {
				set_right(G, right_rotate(P));
			}
			else if(is_right(P) == 0) {
				set_left(G, right_rotate(P));
			}
			else {
				root = right_rotate(P);
			}
			delete_stablize(T);
			return;
		}
	}

	else if(is_right(T) == 0){
		if(get_color(S) == BLACK) {
			if(R == RED) {
				/* case X-2 */
				node_t* G = P->parent;
				if(is_right(P) == 1) {
					set_right(G, left_rotate(P));
				}
				else if(is_right(P) == 0) {
					set_left(G, left_rotate(P));
				}
				else {
					root = left_rotate(P);
				}
				set_color(S, P->color);
				set_color(P, BLACK);
				set_color(R, BLACK);
				return;
			}
			else {
				if(get_color(L) == RED) {
					/* case X-3 */
					set_right(P, right_rotate(S));
					set_color(L, BLACK);
					set_color(S, RED);
					delete_stablize(T);
					return;
				}
				else {
					if(get_color(P) == RED) {
						/* case 1-1 */
						set_color(P, RED);
						set_color(S, BLACK);
						return;
					}
					else {
						/* case 2-1 */
						set_color(S, RED);
						delete_stablize(P);
						return;
					}
				}
			}
		}
		else {
			/* Case 2-4 */
			node_t* G = P->parent;
			if(is_right(P) == 1) {
				set_right(G, left_rotate(P));
			}
			else if(is_right(P) == 0) {
				set_left(G, left_rotate(P));
			}
			else {
				root = left_rotate(P);
			}
			delete_stablize(T);
			return;
		}
	}
	else {
		// unexpeccted
		return;
	}
}

void print_tree(node_t* T, int space) {
	if(T == NULL)
		return;
	else {
		if(space == 0)
			printf("========================================\n");
		print_tree(T->right, space + 1);
		printf("\n");
		for(int i = 0; i < space; i++)
			printf("\t");
		printf("%d%c\n", T->data, (T->color==RED) ? 'R': 'B');
		print_tree(T->left, space + 1);
	}
}


color_t get_color(node_t* T) {
	if(T == NULL)
		return BLACK;
	else
		return T->color;
}

int get_size(node_t* T) {
	if(T == NULL)
		return 0;
	else
		return T->size;
}

node_t* get_sibling(node_t* T) {
	if(is_right(T) == 1)
		return T->parent->left;
	else if(is_right(T) == 0)
		return T->parent->right;
	else
		return NULL;
}

node_t* get_grandparent(node_t* T) {
	if(T->parent == NULL)
		return NULL;
	else
		return T->parent->parent;
}

node_t* get_uncle(node_t* T) {
	return get_sibling(T->parent);
}

node_t* get_one_child(node_t* T) {
	if(T->left == NULL)
		return T->right;
	else
		return T->left;
}

node_t* get_child(node_t* P, direction_t dir) {
	switch(dir) {
		case RIGHT:
			return P->right;
		case LEFT:
			return P->left;
		case NONE:
			return get_one_child(P);
	}
}

/***********************************
 * set functions
 ***********************************/
void set_child(node_t* P, node_t* C, direction_t dir) {
	if(P == NULL)
		return;
	else {
		switch(dir) {
			case RIGHT:
				P->right = C;
				return;
			case LEFT:
				P->left = C;
				return;
			default:
				return;
		}
	}
}


void set_right(node_t* P, node_t* C) {
	if(P == NULL)
		return;
	else
		P->right = C;
}

void set_left(node_t* P, node_t* C) {
	if(P == NULL)
		return;
	else
		P->left = C;
}

void set_parent(node_t* C, node_t* P) {
	if(C == NULL)
		return;
	else
		C->parent = P;
}

void set_color(node_t* T, color_t c) {
	if(T == NULL)
		return;
	else
		T->color = c;
}

void update_size(node_t* T) {
	if(T == NULL)
		return;
	else
		T->size = get_size(T->left) + get_size(T->right) + 1;
}

int is_right(node_t* T) {
	if(T->parent == NULL)
		return -1;
	else if(T->parent->right == T)
		return 1;
	else
		return 0;
}

direction_t which_child(node_t* T) {
	if(T->parent == NULL)
		return NONE;
	else if(T->parent->right == T)
		return RIGHT;
	else
		return LEFT;
}

node_t* rotate(node_t* T, direction_t dir) {
	node_t* X = get_child(T, !dir);
	node_t* Y = get_child(T, dir);
	node_t* C = get_child(X, dir);

	set_child(T, C, !dir);
	set_parent(C, T);

	set_child(X, T, dir);
	set_parent(X, T->parent);
	set_parent(T, X);

	update_size(T);
	update_size(X);
	
	return X;
}

node_t* left_rotate(node_t* T) {
	node_t* tmp = T->right->left;
	
	set_left(T->right, T);
	set_parent(T->right, T->parent);
	set_parent(T, T->right);

	set_right(T, tmp);
	set_parent(tmp, T);

	update_size(T);
	update_size(T->parent);

	return (T->parent);

}

node_t* right_rotate(node_t* T) {
	node_t* tmp = T->left->right;

	set_right(T->left, T);
	set_parent(T->left, T->parent);
	set_parent(T, T->left);

	set_left(T, tmp);
	set_parent(tmp, T);

	update_size(T);
	update_size(T->parent);

	return(T->parent);
}

#endif

