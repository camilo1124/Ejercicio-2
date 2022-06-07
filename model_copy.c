#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int pv = 0;
char anterior[25]; 

void  procesamiento_cadena(char *linea, char **args){

	while (*linea != '\0') {       
		
		while (*linea == ' ' || *linea == '\t' || *linea == '\n')
			*linea++ = '\0';     
		*args++ = linea;          
		while (*linea != '\0' && *linea != ' ' && *linea != '\t' && *linea != '\n') 
			linea++;             
	}

	*args = '\0';                 
}

void  ejecutar(char **args){
	
	pid_t  pid;
	int    status;

	if ((pid = fork()) < 0){ 
		printf("*** ERROR: forking child process failed\n");
		exit(1);
	}
	else if (pid == 0) {
		if (execvp(*args, args) < 0) {     
			printf("*** ERROR: exec failed\n");
			exit(1);
		}
	}
	else {                                 
		while (wait(&status) != pid)   ;
	}

}

void  main(void) {

	char  *linea = malloc(25 * sizeof(char));
	char  *args[64];              

	while (1) {
		printf("osh-> ");     
		gets(linea);          

		if(*linea != ' ' && *linea != '\t' && *linea != '\n' && *linea != '\0'){ 
			if(*linea == '!' && *(++linea) == '!'){

				if (pv == 1){
					procesamiento_cadena(anterior,args);
					ejecutar(args);
				}
				else{
					printf("Sin comandos en el historial\n");
				}

			}else{
				strcpy(anterior,linea);
				procesamiento_cadena(linea, args);       
				if (strcmp(args[0], "exit") == 0)  
					exit(0);            
				ejecutar(args);           
				if(pv==0)pv = 1;
			}
		}

	}
	
     free(linea);
}

