#include "../include/function.h"

int main(int argc, char**argv){
	int my_rank,size,nbp;
	int *tab1,*tab2=NULL,i,*total;

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

	DEBUG_PRINT("begin",my_rank);
	tab1 = create_random_tab(size);
	sort(tab1,size);
	DEBUG_PRINT("sort",my_rank);

	/* first proc */
	if( my_rank == 0 ){
		for(i=0;i<nbp-1;i++){
			MPI_Send(tab1,size,MPI_INT,my_rank+1,TAG,MPI_COMM_WORLD);
			MPI_Recv(tab1,size,MPI_INT,my_rank+1,TAG,MPI_COMM_WORLD,NULL);
		}

		if(verify(tab1,size))
			//DEBUG_PRINT("true",my_rank);
			printf("%d:\tmin: %d\tmax: %d\n",my_rank,tab1[0],tab1[size-1]);

	//	total = (int*)malloc(sizeof(int)*TOTAL);
	/* last proc */
	}else if( my_rank == nbp-1 ){
		tab2 = (int*) malloc (sizeof(int)*size);
		for(i=0;i<nbp-1;i++){
			MPI_Recv(tab2,size,MPI_INT,my_rank-1,TAG,MPI_COMM_WORLD,NULL);
			separate(&tab2,&tab1,size,size);
			MPI_Send(tab2,size,MPI_INT,my_rank-1,TAG,MPI_COMM_WORLD);
		}

		if(verify(tab1,size))
			//DEBUG_PRINT("true",my_rank);
			printf("%d:\tmin: %d\tmax: %d\n",my_rank,tab1[0],tab1[size-1]);
		
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
		
		if(verify(tab1,size))
			//DEBUG_PRINT("true",my_rank);
			printf("%d:\tmin: %d\tmax: %d\n",my_rank,tab1[0],tab1[size-1]);

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

		if(verify(tab1,size))
			//DEBUG_PRINT("true",my_rank);
			printf("%d:\tmin: %d\tmax: %d\n",my_rank,tab1[0],tab1[size-1]);
	}
	
//	MPI_Gather(tab1,size,MPI_INT,total,TOTAL,MPI_INT,0,MPI_COMM_WORLD);

	/* verify 
	if( my_rank == 0 ){
		printf("result: ");
		if(verify(tab2,TOTAL))
			printf("true \n");
		else
			printf("false \n");
	}*/

	free(tab1);
	if(tab2!=NULL)
		free(tab2);
stop:
	/* finish MPI */
	DEBUG_PRINT("fini",my_rank);
	MPI_Finalize();
	
	return 0;
}
