#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define SIZE 10000
#define OP_NUM 40000
#define MAX 9999

int main() {
	int* flag;
	int count = 0;
	int random;
	int op;
	srand(time(NULL));

	for(int i = 0; i < OP_NUM; i++){
		op = rand() % 4;
		switch(op) {
			case 0:
				printf("I %d\n", rand() % MAX + 1);
				break;
			case 1:
				printf("D %d\n", rand() % MAX + 1);
				break;
			case 2:
				printf("S %d\n", rand() % MAX + 1);
				break;
			case 3:
				printf("R %d\n", rand() % MAX + 1);
				break;
		}
	}
	printf("Q\n");
	return 0;
}

