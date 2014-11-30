#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/stat.h>
#include <sys/types.h>

int main()
{
	execlp("./shell","./shell",  NULL); 		
	return 0;
}
