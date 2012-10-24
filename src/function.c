#include "../include/function.h"

double my_gettimeofday(){
  	struct timeval tmp_time;
  	gettimeofday(&tmp_time, NULL);
  	return tmp_time.tv_sec + (tmp_time.tv_usec * 1.0e-6L);
}

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
		if( i % 10000 == 0 )
			DEBUG_PRINT("step",i/10000);
	}
}

int verify(int *tab, int size){
	int i;
	for(i=0;i<size-1;i++)
		if(tab[i]>tab[i+1])
			return 0;
	return 1;
}

void separate(int **tab1,int **tab2,int size1,int size2){
	int *new1,*new2,i=0,j=0;
	new1 = (int*) malloc (sizeof(int)*size1);
	new2 = (int*) malloc (sizeof(int)*size2);

//	DEBUG_PRINT("sep",0);
	while( i+j < size1 ){
		if( i >= size1 )
			new1[i+j] = (*tab2)[j++];
		else if( j >= size2 )
			new1[i+j] = (*tab1)[i++];
		else if( (*tab1)[i] < (*tab2)[j] )
			new1[i+j] = (*tab1)[i++];
		else
			new1[i+j] = (*tab2)[j++];
		
//		printf("i:%d,j:%d,",i,j);
//		print_tab(new1,i+j);
	}

//	DEBUG_PRINT("sep",1);
	while( i+j < (size1+size2) ){
		if( i >= size1 )
			new2[i+j-size1] = (*tab2)[j++];
		else if( j >= size2 )
			new2[i+j-size1] = (*tab1)[i++];
		else if( (*tab1)[i] < (*tab2)[j] )
			new2[i+j-size1] = (*tab1)[i++];
		else
			new2[i+j-size1] = (*tab2)[j++];
		
//		printf("i:%d,j:%d,",i,j);
//		print_tab(new2,i+j-size1);
	}


	free(*tab1);
	free(*tab2);

	*tab1 = new1;
	*tab2 = new2;
}	

int verify2(int *tab1,int *tab2,int size1,int size2){
	if(!verify(tab1,size1))
		return 0;
	if(!verify(tab2,size2))
		return 0;
	if( tab1[size1-1] > tab2[0] )
		return 0;
	return 1;
}
