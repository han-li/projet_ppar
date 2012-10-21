#include "../include/function.h"

void print_tab(int *tab,int size){
	int i;
	for(i=0;i<size;i++)
		printf("%d ",tab[i]);
	printf("\n");
}

int *create_random_tab(int size){
	srand(time(NULL));
	int i;
	int *tab = (int *)malloc(sizeof(int)*size);

	if(tab == NULL){
		perror("malloc");
		exit(1);
	}

	for(i=0;i<size;i++)
		tab[i]=(int)rand();

	return tab;
}

void compare_exchange(int *a,int* b){
	if(*a <= *b)
		return;
	int tmp = *a;
	*a = *b;
	*b = tmp;
}

void sort(int *tab,int size){
	int i,j;
	for(i=0;i<size;i++){
		if(i % 2 == 0)
			for(j=0;j<size-1;j+=2)
				compare_exchange(&tab[j],&tab[j+1]);
		else
			for(j=1;j<size-1;j+=2)
				compare_exchange(&tab[j],&tab[j+1]);
	}
}

int verify(int *tab, int size){
	int i;
	for(i=0;i<size-1;i++)
		if(tab[i]>tab[i+1])
			return 0;
	return 1;
}
