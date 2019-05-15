#include <stdio.h>

#include "hw2_c.h"

#define OP_NUMBER 40000

#define INSERT 0
#define DELETE 1
#define SELECT 2
#define RANK 3

int main() {
	char op;
	int arg;
	int res;
	int cnt = 0;

	int opt_seq[OP_NUMBER];
	int in_seq[OP_NUMBER];
	int out_seq[OP_NUMBER];
		

	init();

	while(1) {
		scanf("%c", &op);
		switch(op) {
			case 'I':
				scanf("%d", &arg); 
				opt_seq[cnt] = INSERT;
				in_seq[cnt] = arg;
				out_seq[cnt] = (res = os_insert(arg));
#ifdef VERBOSE
				printf("%c %d -> %d\n", op, arg, res);
#endif
				cnt++;
				break;
			case 'D':
				scanf("%d", &arg); 
				opt_seq[cnt] = DELETE;
				in_seq[cnt] = arg;
				out_seq[cnt] = (res = os_delete(arg));
#ifdef VERBOSE
				printf("%c %d -> %d\n", op, arg, res);
#endif
				cnt++;
				break;
			case 'S':
				scanf("%d", &arg); 
				opt_seq[cnt] = SELECT;
				in_seq[cnt] = arg;
				out_seq[cnt] = (res = os_select(arg));
#ifdef VERBOSE
				printf("%c %d -> %d\n", op, arg, res);
#endif
				cnt++;
				break;
			case 'R':
				scanf("%d", &arg); 
				opt_seq[cnt] = RANK;
				in_seq[cnt] = arg;
				out_seq[cnt] = (res = os_rank(arg));
#ifdef VERBOSE
				printf("%c %d -> %d\n", op, arg, res);
#endif
				cnt++;
				break;
			case 'Q':
				printf("%s\n", (check(opt_seq, in_seq, out_seq, cnt)) 
						? "CORRECT" 
						: "INCORRECT");
				return 0;
			default:
				continue;
		}

	}
}


