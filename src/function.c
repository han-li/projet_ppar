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
	}
}

int verify(int *tab, int size){
	int i;
	for(i=0;i<size-1;i++)
		if(tab[i]>tab[i+1])
			return 0;
	return 1;
}

void separate(int **tab1,int **tab2,int size){
	int *new1,*new2,i=0,j=0;
	new1 = (int*) malloc (sizeof(int)*size);
	new2 = (int*) malloc (sizeof(int)*size);

//	DEBUG_PRINT("sep",0);
	while( i+j < size ){
		if( (*tab1)[i] < (*tab2)[j] ){
			new1[i+j] = (*tab1)[i];
			i++;
		}else{
			new1[i+j] = (*tab2)[j];
			j++;
		}
	}

//	DEBUG_PRINT("sep",1);
	while( i+j < (size*2) ){
		if( i == size || j == size )break;

		if( (*tab1)[i] < (*tab2)[j] ){
			new2[i+j-size] = (*tab1)[i];
			i++;
		}else{
			new2[i+j-size] = (*tab2)[j];
			j++;
		}
	}

	while( i < size )
		new2[i] = (*tab1)[i++];
	while( j < size )
		new2[j] = (*tab2)[j++];

	free(*tab1);
	free(*tab2);

	*tab1 = new1;
	*tab2 = new2;
}	

int verify2(int *tab1,int *tab2,int size){
	if(!verify(tab1,size))
		return 0;
	if(!verify(tab2,size))
		return 0;
	if( tab1[size-1] > tab2[size-1] )
		return 0;
	return 1;
}
