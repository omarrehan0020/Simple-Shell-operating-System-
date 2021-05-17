#include <stdio.h>
#include <string.h>
#include<sys/wait.h>
#include<unistd.h>
#include <stdlib.h>
#include <time.h>

#define LSH_TOK_BUFSIZE 64
#define LSH_TOK_DELIM " \t\r\n\a"

int size ;


void sig_handler(){
	time_t curtime;
	   struct tm *loc_time;
	   //Getting current time of system
	   curtime = time (NULL);
	   // Converting current time to local time
	   loc_time = localtime (&curtime);
	 pid_t pid;
	  pid = wait(NULL);

	    /* File pointer to hold reference to  file */
	    FILE * fPtr;
	    /* 
	     * Open file in w (write) mode. 
	     * "data/file1.txt" is complete path to create file
	     */
	    fPtr = fopen("file.txt", "w");
	    /* fopen() return NULL if last operation was unsuccessful */
	    if(fPtr == NULL)
	    {
	        /* File not created hence exit */
	        printf("Unable to create file.\n");
	        exit(EXIT_FAILURE);
	    }
	    /* Write data to file */
	    fputs(asctime (loc_time), fPtr);
	    /* Close file to save file data */
	    fclose(fPtr);
}


char **split_line(char *line)
{
  int bufsize = LSH_TOK_BUFSIZE, position = 0;
  char **tokens = malloc(bufsize * sizeof(char*));
  char *token;

  if (!tokens) {
    fprintf(stderr, "allocation error\n");
    exit(EXIT_FAILURE);
  }

  token = strtok(line, LSH_TOK_DELIM);
  while (token != NULL) {
    tokens[position] = token;
    position++;

    if (position >= bufsize) {
      bufsize += LSH_TOK_BUFSIZE;
      tokens = realloc(tokens, bufsize * sizeof(char*));
      if (!tokens) {
        fprintf(stderr, " allocation error\n");
        exit(EXIT_FAILURE);
      }
    }
    token = strtok(NULL, LSH_TOK_DELIM);
  }
  tokens[position] = NULL;
  size = position - 1 ;
  return tokens;
}

int launch(char **args)
{
  pid_t pid, wpid;
  int status;
  int waitt = 1 ;
  if (strcmp(args[0], "exit") == 0)
 		   {
 			   printf("\nGoodbye\n");
 			   exit(0) ;
 		   }
  if (strcmp(args[size], "&") == 0)
 	  {
 		  waitt = 0;
 		  args[size] = NULL ;
 	  }
  pid = fork();
  if (pid == 0) {
    // Child process
	  if (execvp(args[0], args) == -1) 
	  {
          perror("error");
      }
    exit(0);
  } else if (pid < 0) {
    // Error forking
    perror("error");
  } else {
    // Parent process
	   
	  signal(SIGCHLD,sig_handler) ;
      if(waitt)
	   do {
	        wpid = waitpid(pid, &status, WUNTRACED);
	      } while (!WIFEXITED(status) && !WIFSIGNALED(status) );

  }

  return 1;
}



int MAXNLITH = 1024 ;
char prompt1[] ="********************************************************\n";
char prompt2[] ="                    MY SIMPLE SHELL                     \n";
char prompt3[] ="********************************************************\n";



int main() {
	   printf("%s",prompt1);
	   printf("%s",prompt2);
	   printf("%s",prompt3);
	   
	   char **args;
	   int stat=1 ;
	   
	   char cmdline[MAXNLITH] ;
	   while(stat){
		   //read input
		   printf("> ");
		   if((fgets(cmdline,MAXNLITH,stdin)== NULL) && ferror(stdin))
			   perror("fgets error") ;
		   if(feof(stdin)){
			   printf("\n") ;
			   exit(0) ;
		   }
		   cmdline[strlen(cmdline)-1] = '\0' ;
		   args = split_line(cmdline);
		   
		   stat = launch(args) ;
	   }
   
   return 0;
}
