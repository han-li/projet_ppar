#include "../include/function.h"

int main(int argc, char**argv){
	int my_rank,size,nbp;
	int *tab1,*tab2=NULL,i;

	/* init MPI */
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
	MPI_Comm_size(MPI_COMM_WORLD,&nbp);

	srand(time(NULL));
	size = TOTAL / nbp;

	/* exit if fewer than 2 proc */
	if( nbp < 2 ){
		printf("nbr de proc est trop petit !!\n");
		goto stop;
	}

	tab1 = create_random_tab(size);

	/* first proc */
	if( my_rank == 0 ){
		for(i=0;i<nbp-1;i++){
			MPI_Send(tab1,size,MPI_INT,my_rank+1,TAG,MPI_COMM_WORLD);
			MPI_Recv(tab1,size,MPI_INT,my_rank+1,TAG,MPI_COMM_WORLD,NULL);
		}

	/* last proc */
	}else if( my_rank == nbp-1 ){
		tab2 = (int*) malloc (sizeof(int)*size);
		for(i=0;i<nbp-1;i++){
			MPI_Recv(tab2,size,MPI_INT,my_rank-1,TAG,MPI_COMM_WORLD,NULL);
			separate(&tab2,&tab1,size,size);
			MPI_Send(tab2,size,MPI_INT,my_rank-1,TAG,MPI_COMM_WORLD);
		}

		
	/* even rank proc */
	}else if( my_rank % 2 == 0 ){
		tab2 = (int*) malloc (sizeof(int)*size);
		for(i=0;i<nbp-1;i++){
			MPI_Send(tab1,size,MPI_INT,my_rank+1,TAG,MPI_COMM_WORLD);
			MPI_Recv(tab1,size,MPI_INT,my_rank+1,TAG,MPI_COMM_WORLD,NULL);

			MPI_Recv(tab2,size,MPI_INT,my_rank-1,TAG,MPI_COMM_WORLD,NULL);
			separate(&tab1,&tab2,size,size);
			MPI_Send(tab2,size,MPI_INT,my_rank-1,TAG,MPI_COMM_WORLD);
		}
		

	/* odd rank proc */
	}else{
		tab2 = (int*) malloc (sizeof(int)*size);
		for(i=0;i<nbp-1;i++){
			MPI_Recv(tab2,size,MPI_INT,my_rank-1,TAG,MPI_COMM_WORLD,NULL);
			separate(&tab2,&tab1,size,size);
			MPI_Send(tab2,size,MPI_INT,my_rank-1,TAG,MPI_COMM_WORLD);

			MPI_Send(tab1,size,MPI_INT,my_rank+1,TAG,MPI_COMM_WORLD);
			MPI_Recv(tab1,size,MPI_INT,my_rank+1,TAG,MPI_COMM_WORLD,NULL);
		}

	}
	
	free(tab1);
	if(tab2!=NULL)
		free(tab2);
stop:
	/* finish MPI */
	MPI_Finalize();
	
	return 0;
}
