#include "../include/function.h"

int main(int argc,char**argv){

	int size1 = 55, size2 = 3;
	int *tab1 = create_random_tab(size1);
	int *tab2 = create_random_tab(size2);

	sort(tab1,size1);
	sort(tab2,size2);

	separate(&tab1,&tab2,size1,size2);

	if(verify2(tab1,tab2,size1,size2) == 1)
		printf("true\n");
	return 0;
}
