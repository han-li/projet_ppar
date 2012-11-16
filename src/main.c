#include "../include/function.h"

int main(int argc, char**argv){
	int my_rank,nbp,size_last;
	int *tab2=NULL,i,tmp,offset;
	double deb;
	MPI_Request req1;

	/* init MPI */
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
	MPI_Comm_size(MPI_COMM_WORLD,&nbp);

	srand(time(NULL));

	func = &trifusion;

	if(argc>1){
		if(strcmp("trirapide",argv[1])==0)
			func = &trirapide;
		if(strcmp("pram",argv[1])==0)
			func = &sort2;
		if(strcmp("pair_impair",argv[1])==0)
			func = &sort;
		if(strcmp("trifusion",argv[1])==0)
			func = &trifusion;
		if(strcmp("qsort",argv[1])==0)
			func = &my_qsort;
	}

	if(my_rank == 0)
		printf("Nbr PC\tNbr Thread\tAlgorithme\tTime\n%d\t%d\t%s\t",nbp,4,argv[1]);

	/* traitement de compatibilite */
	if( TOTAL % nbp == 0 ){
		sizex = TOTAL / nbp;
		offset = sizex;
		size_last = sizex;
	}
	else{
		sizex = ceil((double)TOTAL/(double)nbp);
		offset = sizex;
		size_last = TOTAL-((nbp-1)*offset);
		if( my_rank == nbp-1 ){
			sizex = size_last;
			DEBUG_PRINT("size_last",size_last);
			DEBUG_PRINT("offset",offset);
		}
			DEBUG_PRINT("sizex",sizex);
	}

	/* exit if fewer than 2 proc */
	if( nbp < 2 ){
		printf("nbr de proc est trop petit !!\n");
		goto stop;
	}

	DEBUG_PRINT("begin",my_rank);
	
	/* init tab */
	tabx = create_random_tab(sizex);

	if( my_rank == 0 )
		deb = my_gettimeofday();
	
	/* sort by threads */
	if(my_rank == nbp-1 && sizex<4)
		goto sad;
	for(i=0;i<4;i++)
		if(pthread_create(&pid[i],NULL,sort_thread,&i)!=0){
			perror("pthread");
			exit(1);
		}

	for(i=0;i<4;i++)
		pthread_join(pid[i],NULL);

	DEBUG_PRINT("sort",my_rank);
	
	separate_thread(tabx,sizex);

	DEBUG_PRINT("gather",my_rank);
sad:
	if(my_rank == nbp -1 && sizex < 4)
		trifusion(tabx,sizex);

	//tmp = (nbp%2 == 0)?(nbp/2):((nbp+1)/2);
	tmp = nbp/2;

	tab2 = (int*) malloc (sizeof(int)*offset);

	/* first proc */
	if( my_rank == 0 ){
		for(i=0;i<tmp;i++){
			MPI_Isend(tabx,sizex,MPI_INT,my_rank+1,TAG,MPI_COMM_WORLD,&req1);
			if(nbp>2){
				MPI_Recv(tab2,sizex,MPI_INT,my_rank+1,TAG,MPI_COMM_WORLD,NULL);
				MPI_Wait(&req1,NULL);
				fusion(tabx,tab2,sizex,sizex);
			}else{
				MPI_Recv(tab2,size_last,MPI_INT,my_rank+1,TAG,MPI_COMM_WORLD,NULL);
				MPI_Wait(&req1,NULL);
				fusion(tabx,tab2,sizex,size_last);
			}
		}

/*		if(verify(tabx,sizex))
			printf("%d:\tmin: %d\tmax: %d\n",my_rank,tabx[0],tabx[sizex-1]);
		else
			DEBUG_PRINT("OH GOD END",my_rank);
*/
	//	total = (int*)malloc(sizeof(int)*TOTAL);
	/* last proc */
	}else if( my_rank == nbp-1 ){
		for(i=0;i<tmp;i++){
			MPI_Isend(tabx,sizex,MPI_INT,my_rank-1,TAG,MPI_COMM_WORLD,&req1);
			MPI_Recv(tab2,offset,MPI_INT,my_rank-1,TAG,MPI_COMM_WORLD,NULL);
			fusion(tab2,tabx,offset,sizex);
		}

/*		if(verify(tabx,sizex))
			//DEBUG_PRINT("true",my_rank);
			printf("%d:\tmin: %d\tmax: %d\n",my_rank,tabx[0],tabx[sizex-1]);
		else
			DEBUG_PRINT("OH GOD END",my_rank);
*/		
	/* even rank proc */
	}else if( my_rank % 2 == 0 ){
		for(i=0;i<tmp;i++){
			
			MPI_Isend(tabx,sizex,MPI_INT,my_rank+1,TAG,MPI_COMM_WORLD,&req1);
			if(my_rank!=nbp-2){
				MPI_Recv(tab2,sizex,MPI_INT,my_rank+1,TAG,MPI_COMM_WORLD,NULL);
				MPI_Wait(&req1,NULL);
				fusion(tabx,tab2,sizex,sizex);
			}else{
				MPI_Recv(tab2,size_last,MPI_INT,my_rank+1,TAG,MPI_COMM_WORLD,NULL);
				MPI_Wait(&req1,NULL);
				DEBUG_PRINT("SIZE_LAST",size_last);
				fusion(tabx,tab2,sizex,size_last);
			}

			MPI_Isend(tabx,sizex,MPI_INT,my_rank-1,TAG,MPI_COMM_WORLD,&req1);
			MPI_Recv(tab2,sizex,MPI_INT,my_rank-1,TAG,MPI_COMM_WORLD,NULL);
			MPI_Wait(&req1,NULL);
			fusion(tab2,tabx,sizex,sizex);
		}

/*		if(verify(tabx,sizex))
			//DEBUG_PRINT("true",my_rank);
			printf("%d:\tmin: %d\tmax: %d\n",my_rank,tabx[0],tabx[sizex-1]);
		else
			DEBUG_PRINT("OH GOD END",my_rank);
*/
	/* odd rank proc */
	}else{
		for(i=0;i<tmp;i++){

			MPI_Isend(tabx,sizex,MPI_INT,my_rank-1,TAG,MPI_COMM_WORLD,&req1);
			MPI_Recv(tab2,sizex,MPI_INT,my_rank-1,TAG,MPI_COMM_WORLD,NULL);
			MPI_Wait(&req1,NULL);
			fusion(tab2,tabx,sizex,sizex);

			MPI_Isend(tabx,sizex,MPI_INT,my_rank+1,TAG,MPI_COMM_WORLD,&req1);
			if(my_rank!=nbp-2){
				MPI_Recv(tab2,sizex,MPI_INT,my_rank+1,TAG,MPI_COMM_WORLD,NULL);
				MPI_Wait(&req1,NULL);
				fusion(tabx,tab2,sizex,sizex);
			}else{
				MPI_Recv(tab2,size_last,MPI_INT,my_rank+1,TAG,MPI_COMM_WORLD,NULL);
				MPI_Wait(&req1,NULL);
				fusion(tabx,tab2,sizex,size_last);
			}
		}

/*		if(verify(tabx,sizex))
			//DEBUG_PRINT("true",my_rank);
			printf("%d:\tmin: %d\tmax: %d\n",my_rank,tabx[0],tabx[sizex-1]);
		else
			DEBUG_PRINT("OH GOD END",my_rank);
*/	}
	
	if( my_rank == 0 ){
		for(i=1;i<nbp;i++)
			MPI_Recv(&tmp,1,MPI_INT,i,TAG,MPI_COMM_WORLD,NULL);
		printf("%f\n",my_gettimeofday()-deb);
	}else
		MPI_Send(&my_rank,1,MPI_INT,0,TAG,MPI_COMM_WORLD);

	/* verify 
	if( my_rank == 0 ){
		printf("result: ");
		if(verify(tab2,TOTAL))
			printf("true \n");
		else
			printf("false \n");
	}*/

	/* IO part */
	DEBUG_PRINT("FILE_OPEN",my_rank);
	MPI_File fh;
	if(MPI_File_open(MPI_COMM_WORLD,"/Vrac/2960481/2960481_test",MPI_MODE_WRONLY | MPI_MODE_CREATE, MPI_INFO_NULL, &fh)!=0){
		perror("MPI_File_open");
		exit(1);
	}

	if(MPI_File_write_at(fh,my_rank*offset*sizeof(int),tabx,sizex,MPI_INT,NULL)!=0){
		perror("MPI_File_write_at");
		exit(1);
	}

	MPI_File_close(&fh);
	DEBUG_PRINT("FILE_CLOSE",my_rank);

	free(tabx);
	if(tab2!=NULL)
		free(tab2);

	/* test result */
	DEBUG_PRINT("TEST RESULT",my_rank);
	if( my_rank == 0 )
		for(i=1;i<nbp;i++)
			MPI_Recv(&tmp,1,MPI_INT,i,TAG,MPI_COMM_WORLD,NULL);
	else
		MPI_Send(&my_rank,1,MPI_INT,0,TAG,MPI_COMM_WORLD);

	if(MPI_File_open(MPI_COMM_WORLD,"/Vrac/2960481/2960481_test",MPI_MODE_RDONLY,MPI_INFO_NULL,&fh)!=0){
		perror("MPI_File_open read");
		exit(1);
	}

	if((tabx = (int*)malloc(sizeof(int)*(sizex+1))) == NULL){
		perror("malloc");
		exit(1);
	}

	DEBUG_PRINT("READ RESULT",my_rank);

	if( my_rank != nbp-1 )
		MPI_File_read_at(fh,my_rank*offset*sizeof(int),tabx,sizex+1,MPI_INT,NULL);
	else
		MPI_File_read_at(fh,(my_rank*offset-1)*sizeof(int),tabx,sizex+1,MPI_INT,NULL);

/*	if(verify(tabx,sizex+1)){
		printf("%d:\tmin: %d\tmax: %d\n",my_rank,tabx[0],tabx[sizex]);
		DEBUG_PRINT("true result",my_rank);
	}else{
		DEBUG_PRINT("false result",my_rank);
		printf("%d:\tmin: %d\tmax: %d\n",my_rank,tabx[0],tabx[sizex]);
	}
*/
	free(tabx);
	MPI_File_close(&fh);
	DEBUG_PRINT("END TEST",my_rank);

stop:
	/* finish MPI */
	DEBUG_PRINT("fini",my_rank);
	MPI_Finalize();
	
	return 0;
}
