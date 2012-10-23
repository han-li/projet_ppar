#include "../include/function.h"

int main(int argc, char**argv){
	int my_rank,size=1572864;

	/* init MPI 
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	*/

	double deb,fin;

	int *tab1 = create_random_tab(size);

	deb = my_gettimeofday();

	sort(tab1,size);

	fin = my_gettimeofday();

	printf("time used: %f",fin-deb);

	if(verify(tab1,size))
		printf("true\n");
	else
		printf("false\n");

	free(tab1);
	
	/* finish MPI
	MPI_Finalize();
	*/
	return 0;
}
