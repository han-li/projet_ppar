#include "../include/function.h"

int main(int argc,char**argv){
	int i;
	sizex = 100000;

	tabx = create_random_tab(sizex);
//	print_tab(tabx,sizex);

	for(i=0;i<4;i++)
		if(pthread_create(&pid[i],NULL,sort_thread,&i)!=0){
			perror("pthread");
			exit(1);
		}

	for(i=0;i<4;i++)
		pthread_join(pid[i],NULL);
	
	//print_tab(tabx,sizex);

	separate_thread(tabx,sizex);
	if(verify(tabx,sizex))
//		print_tab(tabx,sizex);
//	else
		DEBUG_PRINT("HELL YEAH",0);
	return 0;
}
