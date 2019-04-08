#include <stdio.h>
#include <stdlib.h>
#include <time.h>


int partition(int a[], int n, int p);
void insertion_sort(int a[], int n);
void swap(int a[], int i, int j);

//find the "k"th smallest element in array "a" with "n" elements by using Randomized-select in CLRS
int randomized_select(int a[], int n, int k){
	
	int piv;
	srand(time(NULL));
	
	/* error : k is larger then n */
	if(n <= k) {
		printf("ERROR : n <= k\n");
		return -1;
	}

	/* n == 1 : trivial */
	if(n == 1) return a[0];
	
	/* partition with random pivot */
	piv = partition(a, n, (rand() % n));

	/* do it recursively */
	if(piv < k){
		/* k-th element is on the right partition. */
		return randomized_select(
				&a[piv + 1], 
				n - (piv + 1),
				k - piv - 1);		   	
	}
	else if(piv > k){
		/* k-th element is on the left partition. */
		return randomized_select(
				a,
				piv,
				k);
	}
	else {
		/* found k-th element. */
		return a[piv];
	}
}

//find the "k"th smallest element in array "a" with "n" elements by using the worst-case linear-time algorithm in CLRS
int deterministic_select(int a[], int n, int k){
	
	int piv;
	int left = n % 5;
	int blocks = n / 5;
	
	int med;

	/* error : k is larger then n */
	if(n <= k) {
		printf("ERROR : n <= k\n");
		return -1;
	}

	/* n == 1 : trivial */
	if(n == 1) return a[0];
	/* n <= 5 : doen by insertion_sort, since there is only 1 block */
	if(n <= 5) {
		insertion_sort(a, n);
		return a[k];
	}



	/* create a median array */
	med = 0;
	for(int i = n - 5; i >= blocks; i -= 4){
		insertion_sort(&a[i], 5);
		int tmp = a[i + 2];
		a[i + 2] = a[i + 1];
		a[i + 1] = a[i];
		a[i] = a[med];
		a[med] = tmp;
		med++;
	}

	/* find a median of median array */
	med = deterministic_select(a, blocks, blocks / 2);

	/* get the index of median of medians */
	for(int i = 0; i < blocks; i++){
		if(med == a[i]){
			med = i;
			break;
		}
	}
	
	/* partition with median of medians pivot */
	piv = partition(a, n, med);

	/* do it recursively */
	if(piv < k){
		/* k-th element is on the right partition. */
		return deterministic_select(
				&a[piv + 1], 
				n - (piv + 1),
				k - piv - 1);		   	
	}
	else if(piv > k){
		/* k-th element is on the left partition. */
		return deterministic_select(
				a,
				piv,
				k);
	}
	else {
		/* found k-th element. */
		return a[piv];
	}

    return 0;
}

//check whether the "k"th smallest element in array "a" with "n" elements is the "ans"
int checker(int a[], int n, int k, int ans){
	int counter = 0;
	for(int i = 0; i < n; i++){
		if(a[i] < ans) counter++;
	}
   
	return (counter == k);
}

//partition by the pivot in index 'p', and return the pivot point.
int partition(int a[], int n, int p){
	int front = 0;
	int back = n - 2;
	int piv = n - 1;

	swap(a, p, piv);

	while(1){
		while((back >= 0) && (a[back] > a[piv])) back--;
		while((back >= front) && (a[front] < a[piv])) front++;
		if(back > front) {
			swap(a, front, back);
		}
		else {
			swap(a, front, piv);
			break;
		}
	}
	
	return front;
}

void insertion_sort(int a[], int n){
    int min;
    for(int i = 0; i < n - 1; i++){
        min = i;
        for(int j = i + 1; j < n; j++){
            if(a[min] > a[j])
                min = j;
        }
        swap(a, i, min);
    }
}


//swap two element from the integer array by index
void swap(int a[], int i, int j){
	if(i == j)
		return;
	int tmp = a[i];
	a[i] = a[j];
	a[j] = tmp;
}


