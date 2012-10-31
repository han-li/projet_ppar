#include "../include/function.h"

int main(int argc,char**argv){
	int fd = open("hostfile",O_CREAT|O_WRONLY,0644);
	int i;
	char tmp[20]="";

	for(i=1;i<=16;i++){
		if(i<10)
			sscanf(tmp,"ari-31-201-0%d\n",&i);
		else
			sscanf(tmp,"ari-31-201-%d\n",&i);

		write(fd,tmp,sizeof(tmp));
		printf("%s",tmp);
	}

	for(i=1;i<=16;i++){
		if(i<10)
			sscanf(tmp,"ari-31-303-0%d\n",&i);
		else
			sscanf(tmp,"ari-31-303-%d\n",&i);

		write(fd,tmp,sizeof(tmp));
	}

	for(i=1;i<=16;i++){
		if(i<10)
			sscanf(tmp,"ari-31-304-0%d\n",&i);
		else
			sscanf(tmp,"ari-31-304-%d\n",&i);

		write(fd,tmp,sizeof(tmp));
	}

	close(fd);

	return 0;
}
