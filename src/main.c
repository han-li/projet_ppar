#include "../include/function.h"

int main(int argc, char**argv){
	int size=11;
	int *tab = create_random_tab(size);

	print_tab(tab,size);
	sort(tab,size);
	print_tab(tab,size);

	if(verify(tab,size))
		printf("true\n");
	else
		printf("false\n");

	free(tab);
	return 0;
}
