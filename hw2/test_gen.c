#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define SIZE 10000

int main() {
	int* flag;
	int count = 0;
	int random;
	srand(time(NULL));

	flag = calloc(SIZE, sizeof(int));
	while(count < SIZE) {
		random = rand() % SIZE;
		if(flag[random] == 0){
			flag[random] = 1;
			printf("I %d\n", random + 1);
			count++;
		}
	}

	count = 0;
	while(count < SIZE) {
		random = rand() % SIZE;
		if(flag[random] == 1){
			flag[random] = 0;
			printf("D %d\n", random + 1);
			count++;
		}
	}	
	
	printf("Q\n");
	return 0;
}

