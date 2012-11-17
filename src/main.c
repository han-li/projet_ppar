#include "../include/function.h"

int main(int argc, char**argv){
	int my_rank,nbp,size_last;
	int *tab2=NULL,i,tmp,offset;
	double deb;
	MPI_Request req1,req2;

	/* init MPI */
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
	MPI_Comm_size(MPI_COMM_WORLD,&nbp);

	srand(time(NULL));

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
	

	/* sort by openmp */
	func = &trifusion;

	if(argc>1){
		if(strcmp(argv[1],"trifusion")==0)
			func = &trifusion;
		if(strcmp(argv[1],"trirapide")==0)
			func = &trirapide;
		if(strcmp(argv[1],"qsort")==0)
			func = &my_qsort;
		if(strcmp(argv[1],"trifusion")==0)
			func = &trifusion;
	}

	(*func)(tabx,sizex);

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
	}
	else
		MPI_Send(&my_rank,1,MPI_INT,0,TAG,MPI_COMM_WORLD);

	/* Test result */

	int res[2];		// test's token

	if( my_rank == 0 ){
		if(verify(tabx,sizex)){
			res[0] = 1;			// tell next pc i'm ok
			res[1] = tabx[sizex-1];		// my biggest num
		}
		else
			res[0] = 0;
		MPI_Send(res,2,MPI_INT,my_rank+1,TAG_RES,MPI_COMM_WORLD);	// send token to next
		MPI_Recv(res,2,MPI_INT,nbp-1,TAG_RES,MPI_COMM_WORLD,NULL);	// receive after token's circle
		if(res[0])
			printf("true\n");
		else
			printf("false\n");
	}else{
		MPI_Recv(res,2,MPI_INT,my_rank-1,TAG_RES,MPI_COMM_WORLD,NULL);
		if(res[0] == 1 && verify(tabx,sizex) && res[1]<=tabx[0])
			res[1] = tabx[sizex-1];
		else
			res[0] = 0;
		MPI_Send(res,2,MPI_INT,(my_rank+1)%nbp,TAG_RES,MPI_COMM_WORLD);
	}
stop:
	/* finish MPI */
	DEBUG_PRINT("fini",my_rank);
	MPI_Finalize();
	
	return 0;
}
