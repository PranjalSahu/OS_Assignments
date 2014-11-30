#include <stdio.h>
#define MAX 1000

void swap(int *x,int *y)
{
   int temp;
   temp = *x;
   *x = *y;
   *y = temp;
}
void bublesort(int list[], int n)
{
   int i,j;
   for(i=0;i<(n-1);i++)
      for(j=0;j<(n-(i+1));j++)
             if(list[j] > list[j+1])
                    swap(&list[j],&list[j+1]);
}

int main(int argc, char *argv[]){
FILE *f1;
f1 = fopen(argv[1],"r");
int arr[1000]={0};
int count=0, temp, result;

while(1){
	result = fscanf(f1, "%d", &temp);
	if(result == EOF)
	  break;
	else{
	  arr[count] = temp;
	  ++count;
	}  
}

int i;
bublesort(arr,count);

for(i=0;i<count;++i)
    printf("%d ", arr[i]);
 printf("\n");   
return 0;
}
