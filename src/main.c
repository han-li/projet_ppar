#include "../include/function.h"

int main(int argc, char**argv){
	int my_rank,size=11;

	/* init MPI 
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	*/

	int *tab1 = create_random_tab(size);
	int *tab2 = create_random_tab(size);

	print_tab(tab1,size);
	print_tab(tab2,size);
	
	sort(tab1,size);
	sort(tab2,size);
	separate(&tab1,&tab2,size);

	print_tab(tab1,size);
	print_tab(tab2,size);

	if(verify2(tab1,tab2,size))
		printf("true\n");
	else
		printf("false\n");

	free(tab1);
	free(tab2);
	
	/* finish MPI
	MPI_Finalize();
	*/
	return 0;
}
