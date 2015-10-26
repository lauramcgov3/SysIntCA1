/*

Author: Laura McGovern, C12431378
Assignment: Systems Integration, Assignment 1
Date: 19th October 2015

*/


//Main loop
int main (argc int, argv **char)
{
	//Load config files here

	//Run main command loop
	tsh_loop();

	//Shutdown/cleanup.
	return EXIT_SUCCESS;
}


//tsh_loop to read, parse and execute command

void tsh_loop(void)
{

	char *line;
	char **args;
	int status;

	do 
	{
		printf("customshell@usr: "); 
		line = tsh_readline(); //Calls function to read the line
		args = tsh_splitline(); //Calls function to split line into arguments
		status = tsh_execute(args); //Execute arguements 
		
		// Free line and args
		free(line);
		free(args);
	}
	while(status); // Checking status variable
}



