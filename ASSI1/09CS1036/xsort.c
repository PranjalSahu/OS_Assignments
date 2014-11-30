#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>

int main(int argc, char *argv[])
{
        //char *a;
	//a = (char *)malloc(5*sizeof(char));
	//strcpy(a, argv[1]);
	execlp("xterm","xterm", "-hold", "-e", "./sort1", argv[1], NULL); 		

	return 0;
}
