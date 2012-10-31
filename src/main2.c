#include "../include/function.h"

int main(int argc,char**argv){
	int *tab,*ind,*tab2,total,i,j;

	total = 100;
	tab = create_random_tab(total);
	tab2 = (int*)malloc(sizeof(int)*total);
	ind = (int*)malloc(sizeof(int)*total);
	memcpy(tab2,tab,sizeof(int)*total);

	omp_set_num_threads(4);
	
#pragma omp parallel
{
#pragma omp for private(j)
	for(i=0;i<total;i++){
		ind[i] = 0;
		for(j=0;j<total;j++)
			if(tab[i]>tab[j])
				ind[i]++;
	}
}

	memset(tab,-1,sizeof(int)*total);
#pragma omp parallel
{
#pragma omp for private(j)
	for(i=0;i<total;i++){
		j=ind[i];
		while(tab[j]!=-1)j++;
		tab[j] = tab2[i];
	}
}
	free(ind);
	free(tab2);

	if(!verify(tab,total))
		print_tab(tab,total);
	else
		printf("true\n");
	return 0;
}
