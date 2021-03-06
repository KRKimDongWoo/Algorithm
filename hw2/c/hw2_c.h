#ifndef HW2
#define HW2

#include <stdlib.h>

/* enumerate colors */
typedef enum {
	RED,
	BLACK
} color_t;

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

/* Safe set functions.
 * These functions also handle for NULL inputs.
 * If trying to add some family to NULL node, it does nothing.
 */
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
int is_right(node_t* T);

/* Rotate helper functions.
 * Input as a root of the subtree that will be rotated.
 * Returns the new root of the subtree.
 */
node_t* left_rotate(node_t* T);
node_t* right_rotate(node_t* T);

/* Stablize helper functions.
 * Input as a root of the subtree that have to be corrected.
 */
void insert_stablize(node_t* T);
void delete_stablize(node_t* T);

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
		--cnt;
		return x;
	}
	else{
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
						if(is_right(T)) {
							set_left(get_grandparent(T), left_rotate(T->parent));
							insert_stablize(T->left);
							return;	
						}
						else {
							set_right(get_grandparent(T), right_rotate(T->parent));
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
						if(is_right(T)) {
							if(is_right(G) == 1)
								set_right(G->parent, left_rotate(G));
							else if(is_right(G) == 0)
								set_left(G->parent, left_rotate(G));
							else
								root = left_rotate(G);
							
							set_color(G, RED);
							set_color(G->parent, BLACK);
							
							return;
						}
						else {
							if(is_right(G) == 1)
								set_right(G->parent, right_rotate(G));
							else if(is_right(G) == 0)
								set_left(G->parent, right_rotate(G));
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

void update_size(node_t* T) {
	if(T == NULL)
		return;
	else
		T->size = get_size(T->left) + get_size(T->right) + 1;
}


void set_color(node_t* T, color_t c) {
	if(T == NULL)
		return;
	else
		T->color = c;
}

int is_right(node_t* T) {
	if(T->parent == NULL)
		return -1;
	else if(T->parent->right == T)
		return 1;
	else
		return 0;
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

