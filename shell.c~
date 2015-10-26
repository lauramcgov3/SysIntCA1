/*

Author: Laura McGovern, C12431378
Assignment: Systems Integration, Assignment 1
Date: 19th October 2015

*/

#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//Global declarations
#define TSH_RL_BUFFSIZE 1024 //Buffer size for reading a line
#define TSH_TOK_BUFFSIZE 64
#define TSH_TOK_DELIM "\t\r\n\a"

//Main loop
int main(int argc, char **argv)
{
	//Load config files here

	//Run main command loop
	tsh_loop();

	//Shutdown/cleanup.
	return EXIT_SUCCESS;
}


//tsh_loop to call functions to read, parse and execute command

void tsh_loop(void)
{

	char *line;
	char **args;
	int status;

	do 
	{
		printf("usr@customshell: "); 
		line = tsh_readline(); //Calls function to read the line
		args = tsh_splitline(); //Calls function to split line into arguments
		status = tsh_execute(args); //Execute arguements 
		
		// Free line and args
		free(line);
		free(args);
	}
	while(status); // Checking status variable
}


//Function to read a line

char *tsh_readline(void)
{
	int buffsize = TSH_RL_BUFFSIZE; //Set buffer size

	int position = 0;
  	char *buffer = malloc(sizeof(char) * buffsize);
  	int c;

  	if (!buffer) {
    		fprintf(stderr, "lsh: allocation error\n");
    		exit(EXIT_FAILURE);
  	}

  	while (1) {
	    	// Read a character
	    	c = getchar();

	    	// If we hit EOF, replace it with a null character and return.
	   	 if (c == EOF || c == '\n') {
	      		buffer[position] = '\0';
	     	 	return buffer;
	    	} else {
	     	 	buffer[position] = c;
	    	}
	    	position++;

	    	// If we have exceeded the buffer, reallocate.
	    	if (position >= buffsize) {
	      		buffsize += TSH_RL_BUFFSIZE;
	      		buffer = realloc(buffer, buffsize);
	      		if (!buffer) {
				fprintf(stderr, "tsh: allocation error\n");
				exit(EXIT_FAILURE);
			}
		}
  	}
}

//Function for parsing a line into a list of arguments


char **tsh_splitline (char *line)
{
	
	nt buffsize = TSH_TOK_BUFFSIZE, position = 0;
	char **tokens = malloc(buffsize * sizeof(char*));
	char *token;

	if (!tokens) {
	  fprintf(stderr, "lsh: allocation error\n");
	  exit(EXIT_FAILURE);
	}
	
	//Tokenize the string using whitespace as delimiters (strtok library)
	token = strtok(line, TSH_TOK_DELIM);
	while (token != NULL) {
	  tokens[position] = token;
	  position++;
	
	  if (position >= buffsize) {
	    buffsize += TSH_TOK_BUFFSIZE;
	     tokens = realloc(tokens, buffsize * sizeof(char*));
	     if (!tokens) {
		fprintf(stderr, "lsh: allocation error\n");
		exit(EXIT_FAILURE);
	      }
	    }

	    token = strtok(NULL, TSH_TOK_DELIM);
	  }
	  tokens[position] = NULL;
	  return tokens;
}



