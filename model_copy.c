#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int pv = 0;
char anterior[25]; 

void  parse(char *line, char **argv){

	while (*line != '\0') {       
		
		while (*line == ' ' || *line == '\t' || *line == '\n')
			*line++ = '\0';     
		*argv++ = line;          
		while (*line != '\0' && *line != ' ' && *line != '\t' && *line != '\n') 
			line++;             
	}

	*argv = '\0';                 
}

void  execute(char **argv){
	
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

void  main(void) {

	char  *line = malloc(25 * sizeof(char));
	char  *argv[64];              

	while (1) {
		printf("osh-> ");     
		gets(line);          

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
				parse(line, argv);       
				if (strcmp(argv[0], "exit") == 0)  
					exit(0);            
				execute(argv);           
				if(pv==0)pv = 1;
			}
		}

	}
	
     free(line);
}
