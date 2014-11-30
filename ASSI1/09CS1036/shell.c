#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <dirent.h>
#include <time.h>
#include <locale.h>
#include <langinfo.h>
#include <fcntl.h>
#include <pwd.h>
#include <grp.h>
#include <stdint.h>


// executing in other window

char *arglist[20];   // for path variables obtained by tokenizing the string obtained from getenv
char *arglist1[20];  // for variable length arguments for arbitrary command given to shell

// function to make new directory
void mymkdir(char* argv){
  
  if(mkdir(argv, 0777)!=0)
     printf("ERROR making directory\n");  

  return;
}

// function to get current working directory
char *mypwd(){
  char *a, *b;
  b= (char *)malloc(sizeof(char)*100);
  a = getcwd(b, -1);
  printf("%s\n", a);
  return a;
}

//function to change current directory 
void mycd(char *arg){

char *b;
b= (char *)malloc(sizeof(char)*10);
if(chdir(arg)!=0)
  printf("ERROR changing directory\n");
  return;
}

// function to remove directory
void myrmdir(char* argv){
  
  if(rmdir(argv)!=0)
     printf("ERROR removing directory\n");  

  return;
}


// functiuon to tokenize the string
int myfun3(char *mystring, int a){  
  
  int count1=0, count=0, count3=0;
  //char *arglist[10];
  //char a2[10];
  int i;  
  
  if(a==0){
  	for(i=0;i<20;++i)
     		arglist[i] = (char *)malloc(100*sizeof(char));
   }
   else{
       for(i=0;i<20;++i)
                arglist1[i] = (char *)malloc(100*sizeof(char));
   }  
     
     
     
  while(1){                   
       	  count1=0;
       	  //printf("count = %d ", count3);       	  
       	  //printf("%c ",mystring[count] );
	  while(1){
	      if(mystring[count]==10 || mystring[count]==32 || mystring[count]== 58)
	           break;           

     	      if(a==0)
     	           arglist[count3][count1]=mystring[count];     	      
     	      else     
     	           arglist1[count3][count1]=mystring[count];     	      
     	      
     	      
              ++count;
     	      ++count1;
          }
          if(a==0)
             arglist[count3][count1]='\0';
          else
              arglist1[count3][count1]='\0';  
          //printf("%s\n", arglist[count3]);
          
          
          if(mystring[count]==10)
               return count3+1;                                 
 
          ++count;
          ++count3;
  }         
   
  return count3;           // 0 means more than 1 arguement
}


// function to execute in a new process
void executeme(char *mystring){
	int a, status, b;
                
        b = myfun3(mystring, 1);  
        
	// Initialize the argument list 	
	arglist[b]=NULL;	
	a =fork();
	if(a){ 
	  //parent process
	  waitpid(-1, &status, 0);
	}
	else{	     
	     //child process 	     
	     execvp(arglist[0], arglist); 		
	     printf("ERROR executing given command\n");     	  
	}
}

// function for ls command
void myls(char *c){
        struct dirent *a;
        DIR *b;
        b = (DIR *)opendir(c);
        
        a = readdir(b);
        while(a!=NULL){           
           printf("%s\n", a->d_name);
           a = readdir(b);
        }
        return;
}

int myfun2(char *a2, int count, char *mystring){  
  ++count;
  int count1=0;
  while(mystring[count]==32&&count<100)    //ignoring space
        ++count;
        
  while(mystring[count]!=10){
      a2[count1]=mystring[count];
      ++count;
      ++count1;
  }
  a2[count1]='\0';++count1;
  return 0;           // 0 means more than 1 arguement
}

// function to get new command
char *readstring(){
   char *mystring;   
   int by=100, br;
   size_t by1=100;
   mystring = (char *)malloc(by+1);
   br = getline(&mystring, &by1, stdin);
   return mystring;
}

int myfun(char *a1, char *mystring){          // -1 return means only one arguement
  
  int count =0;
  while(count<100){  //checking space charcter       
      if(mystring[count]== 32){
             a1[count] = '\0'; 
             return count; //more than 1             
      }       
      else if(mystring[count]== 10){
             a1[count] = '\0'; 
             return -1;
      }      
      a1[count]=mystring[count];
      ++count;
  }   
  return 0;
}

// function to execute in a new process
int executeme1(char *mystring){
	int a, status;		                        
        
	a =fork();
	if(a){ 
	  //parent process
	  waitpid(-1, &status, 0);
	  return 0;
	}
	else{	     
	     //child process
	     //execlp("xterm","xterm", "-hold", "-e", mystring, NULL); 		
	     execvp(mystring, arglist1); 		
	     //printf("ERROR executing given command\n"); 
	     return -1;    	  
	}
}

// function to execute an arbitrary command given to shell
// by searchin it in eaxh path mentioned in the PATH environment 
// variable
void executeme2(char *mystring, char *whole){
  
  char *check, *ini;
  check = (char *)malloc(sizeof(char)*500);
  //ini = (char *)malloc(sizeof(char)*500);
  strcpy(check,getenv("PATH"));
  check[strlen(getenv("PATH"))] = '\n';
  int num, num1, i;
  
  num = myfun3(check, 0);    
  num1 = myfun3(whole, 1);
  
  arglist1[num1]=NULL;      // for variable length arguments for arbitrary command given to shell
  int h;
  char *b1;
  b1= (char *)malloc(sizeof(char)*100);
  ini = getcwd(b1, -1);
  
  for(i=0;i<num;++i){
  
      // printf("%d>         %s\n", i, arglist[i]);
      mycd(arglist[i]); 
      //mypwd();
      h=executeme1(mystring);
      if(h==-1)
         continue;
      else{ 
           mycd(ini);           
           return;   
           }
     
  }
  
  mycd(ini);
  if(i==num)
      printf("NOT FOUND\n");
  return;
  
}

// function for ls -l command
int lsminusl(){
  struct stat buf;  
  struct tm *tm;
  struct passwd *pwd;
  struct group *grp;
  struct dirent  *dp;
  DIR *b;
  
  char datestring[256];  
  
  b = (DIR *)opendir(".");
  
  while ((dp = readdir(b)) != NULL) {   
  
  if(stat(dp->d_name, &buf)==-1)
     continue;
  
  // file type
  switch (buf.st_mode & S_IFMT) {
    case S_IFBLK:  printf("b");    break;
    case S_IFCHR:  printf("c");    break;
    case S_IFDIR:  printf("d");    break;
    case S_IFIFO:  printf("p");    break;
    case S_IFLNK:  printf("l");    break;
    case S_IFREG:  printf("-");    break;
    case S_IFSOCK: printf("s");    break;
    default:       printf("?");    break;
    }
    
    // owners permissions   
    if(buf.st_mode & S_IRUSR)
       printf("r");
    else printf("-");
    
    if(buf.st_mode & S_IWUSR)
       printf("w");
    else printf("-");
   
    if(buf.st_mode & S_IXUSR)
       printf("x");
    else printf("-");
    
    // group permissions   
    if(buf.st_mode & S_IRGRP)
       printf("r");
    else printf("-");
    
    if(buf.st_mode & S_IWGRP)
       printf("w");
    else printf("-");
   
    if(buf.st_mode & S_IXGRP)
       printf("x");
    else printf("-");
      
    // other permissions   
    if(buf.st_mode & S_IROTH)
       printf("r");
    else printf("-");
    
    if(buf.st_mode & S_IWOTH)
       printf("w");
    else printf("-");
   
    if(buf.st_mode & S_IXOTH)
       printf("x    ");
    else printf("-    ");
      
    printf("%4d  ", (int)buf.st_nlink);  
    
    
  // group user's name
  if ((grp = getgrgid(buf.st_gid)) != NULL)
        printf("%-8.8s  ", grp->gr_name);
    else
        printf("%-8d  ", buf.st_gid);

  // owner's name
    if ((pwd = getpwuid(buf.st_uid)) != NULL)
        printf("%-8.8s  ", pwd->pw_name);
    else
        printf("%-8d  ", buf.st_uid);
 
  printf("%8jd  ", (intmax_t)buf.st_size);  
  tm = localtime(&buf.st_mtime);
  strftime(datestring, sizeof(datestring), nl_langinfo(D_T_FMT), tm);
  printf("%s  ", datestring);
  

  printf("%-20s\n", dp->d_name );
  }  
  return 0;
}




int main()
{
        char b1[10], b2[10];
        int check, check1;
        char *mystring;                   
        
	while(1){
		printf("myshell$ ");		
		mystring=readstring();
		check=myfun(b1, mystring);
		if(strcmp(b1,"pwd")==0){
		     mypwd();
		}
		else if(strcmp(b1,"cd")==0){
		     if(check==-1){
		         printf("Not enough arguement\n");
		         continue; 
		     }
		     check1=myfun2(b2, check,mystring); 
		     mycd(b2);
		}
		else if(strcmp(b1,"mkdir")==0){
		     if(check==-1){
		         printf("Not enough arguement\n");
		         continue; 
		     } 
		     check1=myfun2(b2, check,mystring); 
		     mymkdir(b2);
		}
		else if(strcmp(b1,"rmdir")==0){
		     if(check==-1){
		         printf("Not enough arguement\n");
		         continue; 
		     }
		     check1=myfun2(b2, check,mystring); 
		     myrmdir(b2);
		}
		else if(strcmp(b1,"ls")==0){
		     if(check==-1)
    		         myls(mypwd());
    		     else {
    		          check1=myfun2(b2, check,mystring); 
    		          if(strcmp(b2,"-l")==0) 
    		              lsminusl();
    		          else{
    		              printf("Wrong usage of ls -l\n");    
    		              continue;
    		              }
    		     }     
		}
		else if(strcmp(b1,"exit")==0){
		     return 0;
		}     
		else{		     
		     executeme2(b1, mystring);
	        }
	 }      
	return 0;
}
