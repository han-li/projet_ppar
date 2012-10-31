#include "../include/function.h"

int main(int argc,char**argv){
	MPI_Init(&argc,&argv);

	int *tab,size=50;
	tab = create_random_tab(size);

	sort_omp(tab,size);

	if(verify(tab,size))
		printf("true\n");
	else
		print_tab(tab,size);

	MPI_Finalize();
	return 0;
}
