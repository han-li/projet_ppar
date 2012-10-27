#include "../include/function.h"

int main(int argc, char**argv){
	int my_rank,nbp;
	int *tab2=NULL,i,*total;

	/* init MPI */
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
	MPI_Comm_size(MPI_COMM_WORLD,&nbp);

	srand(time(NULL));
	sizex = TOTAL / nbp;

	/* exit if fewer than 2 proc */
	if( nbp < 2 ){
		printf("nbr de proc est trop petit !!\n");
		goto stop;
	}

	DEBUG_PRINT("begin",my_rank);
	
	/* init tab */
	tabx = create_random_tab(sizex);
	
	/* sort by threads */
	for(i=0;i<4;i++)
		if(pthread_create(&pid[i],NULL,sort_thread,&i)!=0){
			perror("pthread");
			exit(1);
		}

	for(i=0;i<4;i++)
		pthread_join(pid[i],NULL);
	
	separate_thread(tabx,sizex);

	DEBUG_PRINT("sort",my_rank);

	/* first proc */
	if( my_rank == 0 ){
		for(i=0;i<nbp-1;i++){
			MPI_Send(tabx,sizex,MPI_INT,my_rank+1,TAG,MPI_COMM_WORLD);
			MPI_Recv(tabx,sizex,MPI_INT,my_rank+1,TAG,MPI_COMM_WORLD,NULL);
		}

		if(verify(tabx,sizex))
			printf("%d:\tmin: %d\tmax: %d\n",my_rank,tabx[0],tabx[sizex-1]);
		else
			DEBUG_PRINT("OH GOD END",my_rank);

	//	total = (int*)malloc(sizeof(int)*TOTAL);
	/* last proc */
	}else if( my_rank == nbp-1 ){
		tab2 = (int*) malloc (sizeof(int)*sizex);
		for(i=0;i<nbp-1;i++){
			MPI_Recv(tab2,sizex,MPI_INT,my_rank-1,TAG,MPI_COMM_WORLD,NULL);
			separate(&tab2,&tabx,sizex,sizex);
			MPI_Send(tab2,sizex,MPI_INT,my_rank-1,TAG,MPI_COMM_WORLD);
		}

		if(verify(tabx,sizex))
			//DEBUG_PRINT("true",my_rank);
			printf("%d:\tmin: %d\tmax: %d\n",my_rank,tabx[0],tabx[sizex-1]);
		else
			DEBUG_PRINT("OH GOD END",my_rank);
		
	/* even rank proc */
	}else if( my_rank % 2 == 0 ){
		tab2 = (int*) malloc (sizeof(int)*sizex);
		for(i=0;i<nbp-1;i++){
			MPI_Send(tabx,sizex,MPI_INT,my_rank+1,TAG,MPI_COMM_WORLD);
			MPI_Recv(tabx,sizex,MPI_INT,my_rank+1,TAG,MPI_COMM_WORLD,NULL);

			MPI_Recv(tab2,sizex,MPI_INT,my_rank-1,TAG,MPI_COMM_WORLD,NULL);
			separate(&tab2,&tabx,sizex,sizex);
			MPI_Send(tab2,sizex,MPI_INT,my_rank-1,TAG,MPI_COMM_WORLD);
		}
		
		if(verify(tabx,sizex))
			//DEBUG_PRINT("true",my_rank);
			printf("%d:\tmin: %d\tmax: %d\n",my_rank,tabx[0],tabx[sizex-1]);
		else
			DEBUG_PRINT("OH GOD END",my_rank);

	/* odd rank proc */
	}else{
		tab2 = (int*) malloc (sizeof(int)*sizex);
		for(i=0;i<nbp-1;i++){
			MPI_Recv(tab2,sizex,MPI_INT,my_rank-1,TAG,MPI_COMM_WORLD,NULL);
			separate(&tab2,&tabx,sizex,sizex);
			MPI_Send(tab2,sizex,MPI_INT,my_rank-1,TAG,MPI_COMM_WORLD);

			MPI_Send(tabx,sizex,MPI_INT,my_rank+1,TAG,MPI_COMM_WORLD);
			MPI_Recv(tabx,sizex,MPI_INT,my_rank+1,TAG,MPI_COMM_WORLD,NULL);
		}

		if(verify(tabx,sizex))
			//DEBUG_PRINT("true",my_rank);
			printf("%d:\tmin: %d\tmax: %d\n",my_rank,tabx[0],tabx[sizex-1]);
		else
			DEBUG_PRINT("OH GOD END",my_rank);
	}
	
//	MPI_Gather(tabx,sizex,MPI_INT,total,TOTAL,MPI_INT,0,MPI_COMM_WORLD);

	/* verify 
	if( my_rank == 0 ){
		printf("result: ");
		if(verify(tab2,TOTAL))
			printf("true \n");
		else
			printf("false \n");
	}*/

	MPI_File fh;
	if(MPI_File_open(MPI_COMM_WORLD,"/Vrac/2960481_test",MPI_MODE_WRONLY | MPI_MODE_CREATE, MPI_INFO_NULL, &fh)!=0){
		perror("MPI_File_open");
		exit(1);
	}

	if(MPI_File_write_at(fh,my_rank*sizex*sizeof(int),tabx,sizex,MPI_INT,NULL)!=0){
		perror("MPI_File_write_at");
		exit(1);
	}

	MPI_File_close(&fh);

	free(tabx);
	if(tab2!=NULL)
		free(tab2);
stop:
	/* finish MPI */
	DEBUG_PRINT("fini",my_rank);
	MPI_Finalize();
	
	return 0;
}
