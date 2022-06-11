#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

#define MAX_LINE 80

int pv = 0;
char anterior[25]; 

void  procesamiento_cadena(char *linea, char **args, int *f){

	while (*linea != '\0') {       
		
		while (*linea == ' ' || *linea == '\t' || *linea == '\n'){
			*linea++ = '\0';     
		}
		if (*linea != '&'){	
			*args++ = linea;          
		}else {
			*f=1;
		}
		while (*linea != '\0' && *linea != ' ' && *linea != '\t' && *linea != '\n') 
			linea++;             
	}

	*args = '\0';                 
}

void  ejecutar(char **args, int *f){
	
	pid_t  pid;
	pid = fork();

	if (pid < 0){
		fprintf(stderr, "Fork Failed");
	} else if (pid == 0){
		if(execvp(args[0],args) == -1) printf("Error ejecutando el comando\n");
	} else{
		if (*f == 0){ 
			wait(NULL);

		}

	}

}

void  main(void) {

	char  *linea = malloc(((MAX_LINE/2)+1) * sizeof(char));
	char  *args[MAX_LINE/2+1];              
	int should_run = 1; 
	int *f = malloc(sizeof(int));

	while (should_run) {
		printf("osh-> ");     
		fflush(stdout);
		gets(linea);          

		if(*linea != ' ' && *linea != '\t' && *linea != '\n' && *linea != '\0'){ 
			if(*linea == '!' && *(++linea) == '!'){

				if (pv == 1){
					printf("Comando anterior: \n");
					printf("%s\n",anterior);
					procesamiento_cadena(anterior,args,f);
					ejecutar(args,f);
				}
				else{
					printf("Sin comandos en el historial\n");
				}

			}else{
				strcpy(anterior,linea);
				procesamiento_cadena(linea, args,f);       
				if (strcmp(args[0], "exit") == 0){
					should_run = 0;            
				} else{
					ejecutar(args,f);           
					if(pv==0)pv = 1;
				}
			}
		}

	} 
}

