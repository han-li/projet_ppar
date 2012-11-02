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
//		tab[i]=(int)rand()%10;

	return tab;
}

void compare_exchange(int *a,int* b){
	if(*a <= *b)
		return;
	int tmp = *a;
	*a = *b;
	*b = tmp;
}

void exchange(int *a,int* b){
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

void sort2(int *tab,int size){
	int *ind = (int*)malloc(sizeof(int)*size);
       	int *new_tab = (int*)malloc(sizeof(int)*size);
	int i,j;
	memcpy(new_tab,tab,sizeof(int)*size);

	for(i=0;i<size;i++){
		ind[i] = 0;
		for(j=0;j<size;j++)
			if(tab[i]>tab[j])
				ind[i]++;
		if( i % 10000 == 0 )
			DEBUG_PRINT("step",i/10000);
	}

	memset(tab,-1,sizeof(int)*size);
	for(i=0;i<size;i++){
		j=ind[i];
		while(tab[j]!=-1)j++;
		tab[j] = new_tab[i];
	}

	free(ind);
	free(new_tab);
}

void sort_omp(int *tab,int size){
	int *ind = (int*)malloc(sizeof(int)*size);
       	int *new_tab = (int*)malloc(sizeof(int)*size);
	int i,j;
	memcpy(new_tab,tab,sizeof(int)*size);

	omp_set_num_threads(4);
//	omp_set_nested(1);

#pragma omp parallel
{
#pragma omp for private(j)
	for(i=0;i<size;i++){
//#pragma omp task private(j)
//		{
		ind[i] = 0;
//#pragma omp parallel
//#pragma omp for
		for(j=0;j<size;j++)
			if(tab[i]>tab[j])
				ind[i]++;
		if( i % 10000 == 0 )
			DEBUG_PRINT("step",i/10000);
//		}
	}
}

	memset(tab,-1,sizeof(int)*size);

#pragma omp parallel
{
#pragma omp for private(j)
	for(i=0;i<size;i++){
		j=ind[i];
		while(tab[j]!=-1)j++;
		tab[j] = new_tab[i];
	}
}

	free(ind);
	free(new_tab);
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

void scinder(int *tab,int size,int **tab2,int *size1,int *size2){
	*size1 = size/2;
	*size2 = size-*size1;
	
	*tab2 = &tab[*size1];
}

void fusion(int *tab1,int *tab2,int size1,int size2){
	int *new1,*new2,i=0,j=0;
	new1 = (int*)malloc(sizeof(int)*size1);
	new2 = (int*)malloc(sizeof(int)*size2);
	
	memcpy(new1,tab1,sizeof(int)*size1);
	memcpy(new2,tab2,sizeof(int)*size2);


	while( i+j < size1 ){
		if( i >= size1 )
			tab1[i+j] = new2[j++];
		else if( j >= size2 )
			tab1[i+j] = new1[i++];
		else if( new1[i] < new2[j] )
			tab1[i+j] = new1[i++];
		else
			tab1[i+j] = new2[j++];
		
	}

	while( i+j < (size1+size2) ){
		if( i >= size1 )
			tab2[i+j-size1] = new2[j++];
		else if( j >= size2 )
			tab2[i+j-size1] = new1[i++];
		else if( new1[i] < new2[j] )
			tab2[i+j-size1] = new1[i++];
		else
			tab2[i+j-size1] = new2[j++];
	}

	free(new1);
	free(new2);
}

void trifusion_tmp(int *tab,int size){
	DEBUG_PRINT("thread",omp_get_thread_num());
	if(size <= 1)
		return;
	
	int *tab2,size1,size2;
	
	scinder(tab,size,&tab2,&size1,&size2);
#pragma omp task
	trifusion(tab,size1);
#pragma omp task
	trifusion(tab2,size2);
#pragma omp taskwait
	fusion(tab,tab2,size1,size2);
}

void trifusion(int *tab,int size){
	omp_set_num_threads(4);

#pragma omp parallel
#pragma omp single
	trifusion_tmp(tab,size);
}

int choix_pivot(int *tab,int first,int last){
	srand(time(NULL));
	int ind = rand()%(last-first+1) + first;
	return ind;
}

int partitionner(int *tab,int first,int last,int pivot){
	exchange(&tab[last],&tab[pivot]);
	int i,j = first;

	for(i=first;i<last;i++)
		if(tab[i]<tab[last]){
			exchange(&tab[i],&tab[j]);
			j++;
		}

	exchange(&tab[j],&tab[last]);
	return j;
}

void trirapide_tmp(int *tab,int first,int last){
	int pivot;
	if(first<last){
		DEBUG_PRINT("thread",omp_get_thread_num());
		pivot = choix_pivot(tab,first,last);
		DEBUG_PRINT("PIVOT",pivot);
		//print_tab(&tab[first],last-first+1);
		pivot = partitionner(tab,first,last,pivot);
		DEBUG_PRINT("PIVOT",pivot);
		//print_tab(&tab[first],last-first+1);
#pragma omp task
		trirapide_tmp(tab,first,pivot-1);
#pragma omp task
		trirapide_tmp(tab,pivot+1,last);
	}
}

void trirapide(int *tab,int size){
	omp_set_num_threads(4);
#pragma omp parallel
#pragma omp single
	trirapide_tmp(tab,0,size-1);
#pragma omp taskwait
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

void *sort_thread(void* arg){
	int ssize,ind,i,sssize;

	for(i=0;i<4;i++)
		if( pid[i] == pthread_self() )
			ind = i;

	if( sizex%4 == 0 ){
		ssize = sizex/4;
		sort2(tabx+(ind*ssize),ssize);
		if(!verify(tabx+(ind*ssize),ssize))
			DEBUG_PRINT("SORT_ERROR",1);
		
	}else{
		ssize = ceil((double)sizex/4.);
		if( ind != 3 ){
			sort2(tabx+(ind*ssize),ssize);
			if(!verify(tabx+(ind*ssize),ssize))
				DEBUG_PRINT("SORT_ERROR",2);
		}
		else{
			sssize = sizex - 3*ssize;	
			sort2(tabx+(ind*ssize),sssize);
			if(!verify(tabx+(ind*ssize),sssize))
				DEBUG_PRINT("SORT_ERROR",3);
		}
	}

	pthread_exit(arg);
	return arg;
}

void separate_thread(int *tab,int size){
	int i,j,k,l,ssize,sssize;
//	int *tab2 = (int*)malloc(sizeof(int)*size);

//	memcpy(tab2,tab,sizeof(int)*size);
//	for(i=0;i<size;i++)
//		tab2[i] = tab[i];

	if( size%4 == 0 )
		ssize = sssize = size/4;
	else{
		ssize = ceil((double)size/4.);
		DEBUG_PRINT("CEIL",ssize);
		sssize = size - 3*ssize;
	}

//	i=0;j=ssize;k=ssize*2;l=ssize*3;

	fusion(tab,&tab[ssize],ssize,ssize);
	fusion(&tab[2*ssize],&tab[3*ssize],ssize,sssize);
	fusion(tab,&tab[2*ssize],2*ssize,ssize+sssize);
/*
	DEBUG_PRINT("BEGIN",1);
	while( i+(j-ssize) < 2*ssize ){
		if( i >= ssize )
			tab[i+(j-ssize)] = tab2[j++];
		else if ( j >= 2*ssize)
			tab[i+(j-ssize)] = tab2[i++];
		else if (tab2[i] > tab2[j])
			tab[i+(j-ssize)] = tab2[j++];
		else
			tab[i+(j-ssize)] = tab2[i++];

		if(!verify(tab,i+j-ssize)){
			DEBUG_PRINT("i",i);
			DEBUG_PRINT("j",j);
			exit(1);
		}
	}

	while( k+l-5*ssize < size-2*ssize){
		if( k >= 3*ssize )
			tab[k+l-3*ssize] = tab2[l++];
		else if ( l >= size )
			tab[k+l-3*ssize] = tab2[k++];
		else if ( tab2[k] > tab2[l] )
			tab[k+l-3*ssize] = tab2[l++];
		else
			tab[k+l-3*ssize] = tab2[k++];

		if(!verify(tab+2*ssize,k+l-3*ssize)){
			DEBUG_PRINT("k-1",tab[k-1]);
			DEBUG_PRINT("l",tab[l]);
			exit(1);
		}
	}

	if(!verify(tab,ssize*2)){
		DEBUG_PRINT("OH GOD",1);
	}
	if(!verify(tab+ssize*2,size-2*ssize)){
		DEBUG_PRINT("OH GOD",2);
	}

	i=0;k=2*ssize;
	memcpy(tab2,tab,sizeof(int)*size);
//	for(i=0;i<size;i++)
//		tab2[i] = tab[i];

	while( i+(k-2*ssize) < size ){
		if( i >= 2*ssize )
			tab[i+(k-2*ssize)] = tab2[k++];
		else if( k >= size )
			tab[i+(k-2*ssize)] = tab2[i++];
		else if( tab2[i] > tab2[k] )
			tab[i+(k-2*ssize)] = tab2[k++];
		else
			tab[i+(k-2*ssize)] = tab2[i++];
	}

	free(tab2);
*/
}

