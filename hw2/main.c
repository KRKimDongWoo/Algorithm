#include <stdio.h>

#include "hw2_c.h"

extern node_t* root;

int main() {
	char op;
	int arg;

	init();

	while(1) {
		scanf("%c", &op);
		switch(op) {
			case 'I':
				scanf("%d", &arg);
				os_insert(arg);
				break;
			case 'D':
				scanf("%d", &arg);
				os_delete(arg);
				break;
			case 'S':
				scanf("%d", &arg);
				printf("%d\n", os_select(arg));
				break;
			case 'R':
				scanf("%d", &arg);
				printf("%d\n", os_rank(arg));
				break;
			case 'Q':
				return 0;
			default:
				continue;
		}

	}
}


