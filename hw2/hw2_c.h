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
node_t* get_child(node_t* P, direction_t dir);
node_t* get_sibling(node_t* T);
node_t* get_grandparent(node_t* T);
node_t* get_uncle(node_t* T);

/* Safe set functions.
 * These functions also handle for NULL inputs.
 * If trying to add some family to NULL node, it does nothing.
 */
void set_child(node_t*P, node_t* C, direction_t dir);
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

/* Rotate helper functions.
 * Input as a root of the subtree that will be rotated.
 * Returns the new root of the subtree.
 */
node_t* rotate(node_t* T, direction_t dir);

/* Stablize helper functions.
 * Input as a root of the subtree that have to be corrected.
 */
void insert_stablize(node_t* T);
void delete_stablize(node_t* P, direction_t dir);

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
	//print_tree(root, 0);
	return x;
}

int os_delete(int x){

    int k = 0;
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
		int flag = 0;
		node_t* curr = root;
		node_t* target;

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
			 */			target = curr->left;
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
		if((get_color(target) == BLACK) && (get_color(get_child(target, NONE)) == BLACK)){
			/* Target and its child are both black.
			 * Problem occured, so set flag.
			 */
			flag = 1;
		}

		node_t* P = target->parent;
		node_t* C = get_child(target, NONE);
		direction_t target_dir = which_child(target);

		set_child(P, C, target_dir);
		set_parent(C, P);
		set_color(C, BLACK);

		free(target);

		if(flag){
			delete_stablize(P, target_dir);
		}

		--cnt;
		//print_tree(root, 0);
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
				} /* Case 1 */
				else {
					/* Case 2
					 * T is red, p is red and s is black.
					 * Rotate according to current location.
					 */
					if(which_child(T) ^ which_child(T->parent)) {
						/* Case 2-1
						 * T and parent grown in different direction.
						 * Rotate and stablize recursively.
						 * It will call case 2-2 in result.
						 */
						node_t* G = get_grandparent(T);
						direction_t dir = !which_child(T);

						set_child(G, rotate(T->parent, dir), dir);
						insert_stablize(get_child(T, dir));
					} /* Case 2-1 */
					else {
						/* Case 2-2
						 * T and parent grow in same direction.
						 * Rotate and change color.
						 */
						node_t* G = get_grandparent(T);
						node_t* PG = G->parent;

						if(PG == NULL) {
							root = rotate(G, !which_child(T));
						}
						else {
							direction_t G_dir = which_child(G);
							direction_t T_dir = which_child(T);
							set_child(PG, rotate(G, !T_dir), G_dir);
						}
						set_color(G, RED);
						set_color(G->parent, BLACK);
					} /* Case 2-2 */
				} /* Case 2 */
			} /* Parent is red. */
		} /* T is red */
	}
}

void delete_stablize(node_t* P, direction_t dir) {
	if(P == NULL)
		return;

	node_t* X = get_child(P, dir);
	node_t* S = get_child(P, !dir);
	node_t* SI = get_child(S, dir);
	node_t* SO = get_child(S, !dir);

	if(get_color(S) == BLACK) {
		if(get_color(SO) == RED) {
			/* case X-2 */
			node_t* G = P->parent;
			direction_t P_dir = which_child(P);
			if(G == NULL)
				root = rotate(P, dir);
			else
				set_child(G, rotate(P, dir), P_dir);

			set_color(S, get_color(P));
			set_color(P, BLACK);
			set_color(SO, BLACK);
		}
		else {
			if(get_color(SI) == RED) {
				/* case X-3 */
				set_child(P, rotate(S, !dir), !dir);

				set_color(SI, BLACK);
				set_color(S, RED);

				delete_stablize(P, dir);
			}
			else {
				if(get_color(P) == RED) {
					/* case 1-1 */
					set_color(P, BLACK);
					set_color(S, RED);
				}
				else {
					/* case 2-1 */
					set_color(S, RED);
					if(P->parent != NULL)
						delete_stablize(P->parent, which_child(P));
				}
			}
		}
	}
	else {
		/* case 2-4 */
		node_t* G = P->parent;
		direction_t P_dir = which_child(P);
		if(G == NULL)
			root = rotate(P, dir);
		else
			set_child(G, rotate(P, dir), P_dir);
		
		set_color(P, RED);
		set_color(S, BLACK);

		delete_stablize(P, dir);
	}
	return;
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
		printf("%d%c(%d)\n", T->data, (T->color==RED) ? 'R': 'B', T->size);
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
	if(T->parent == NULL)
		return NULL;
	else
		return get_child(T->parent, !which_child(T));
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
	if(P == NULL) {
		root = C;
		return;
	}
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
	if(T == NULL)
		return NONE;
	else if(T->parent == NULL)
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

