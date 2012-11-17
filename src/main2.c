#include "../include/function.h"

int main(int argc,char**argv){
	double deb,i;
	sizex = TOTAL;
	func = &trifusion;

	if(argc>1){
		if(strcmp(argv[1],"trifusion")==0)
			func = &trifusion;
		if(strcmp(argv[1],"trirapide")==0)
			func = &trirapide;
		if(strcmp(argv[1],"qsort")==0)
			func = &my_qsort;
	}

	for(i=1;i<=4;i++){	
		omp_set_num_threads(i);
		tabx = create_random_tab(sizex);
		deb = my_gettimeofday();
		(*func)(tabx,sizex);
		printf("%f\n",my_gettimeofday()-deb);
		free(tabx);
	}
/*
	if(verify(tabx,sizex))
		printf("true\n");
	else
		printf("false\n");
*/
	return 0;
}
