#include "../include/function.h"

int main(int argc,char**argv){
	int *tab,size=21;
	tab = create_random_tab(size);
	print_tab(tab,size);
	trirapide(tab,size);
	print_tab(tab,size);
	if(verify(tab,size))
		printf("true\n");
	else
		printf("false\n");

	return 0;
}
