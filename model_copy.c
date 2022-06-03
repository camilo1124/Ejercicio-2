#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int pv = 0;
char anterior[25]; 

void  parse(char *line, char **argv)
{
     while (*line != '\0') {       /* if not the end of line ....... */ 
          while (*line == ' ' || *line == '\t' || *line == '\n')
               *line++ = '\0';     /* replace white spaces with 0    */
          *argv++ = line;          /* save the argument position     */
          while (*line != '\0' && *line != ' ' && 
                 *line != '\t' && *line != '\n') 
               line++;             /* skip the argument until ...    */
     }
     *argv = '\0';                 /* mark the end of argument list  */
}

void  execute(char **argv)
{
     pid_t  pid;
     int    status;

     if ((pid = fork()) < 0){ 
	  printf("*** ERROR: forking child process failed\n");
          exit(1);
     }
     else if (pid == 0) {
	     if (execvp(*argv, argv) < 0) {     
               printf("*** ERROR: exec failed\n");
               exit(1);
	     }
     }
     else {                                 
          while (wait(&status) != pid)   ;
     }
     
}

void  main(void)
{
     char  *line = malloc(25 * sizeof(char));             /* the input line                 */
     char  *argv[64];              /* the command line argument      */


     while (1) {                   /* repeat until done ....         */
          printf("osh-> ");     /*   display a prompt             */
          gets(line);              /*   read in the command line     */
	  if(*line != ' ' && *line != '\t' && *line != '\n' && *line != '\0'){ 
		  if(*line == '!' && *(++line) == '!'){
			  if (pv == 1){
				  parse(anterior,argv);
				  execute(argv);
			  }
			  else{
				  printf("Sin comandos en el historial\n");
			  }
		  
		  }else{
			  strcpy(anterior,line);
			  parse(line, argv);       /*   parse the line               */
			  if (strcmp(argv[0], "exit") == 0)  /* is it an "exit"?     */
			       exit(0);            /*   exit if it is                */
			  execute(argv);           /* otherwise, execute the command */
			  if(pv==0)pv = 1;
		  }
	  }
     }
     free(line);
}
