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


//Function declarations for internal commands 
int tsh_cd(char **args);
int tsh_help(char **args);
int tsh_exit(char **args);

//List the internal commands, followed by a list of the functions to go with those commands.
char *builtin_str[] = 
{
    "cd",
    // "dt",
    //"ud",
    "help",
    "exit"
};

int (*builtin_func[]) (char **) = 
{
    &tsh_cd,
    //&tsh_dt,
    //&tsh_ud,
    &tsh_help,
    &tsh_exit
};

int tsh_num_builtins() 
{
    return sizeof(builtin_str) / sizeof(char *);
}


//Internal command implementation


//cd function
int tsh_cd(char **args)
{
    if (args[1] == NULL) 
    {
        fprintf(stderr, "tsh: expected argument to \"cd\"\n");
    } 
    else 
    {
        if (chdir(args[1]) != 0) 
        {
            perror("tsh");
        }
    }
    return 1;
}


//Help function
int tsh_help(char **args)
{
    int i;
    printf("Laura McGovern's custom shell \n");
    printf("Type program names and arguments, and hit enter.\n");
    printf("The following are built in:\n");

    for (i = 0; i < tsh_num_builtins(); i++) 
    {
        printf("  %s\n", builtin_str[i]);
    }

    printf("Use the man command for information on other programs.\n");
    return 1;
}


//Exit function
int tsh_exit(char **args)
{
    return 0;
}


//Function to launch shell processes
int tsh_launch(char **args)
{
    pid_t pid, wpid;
    int status;

    pid = fork();
    if (pid == 0) 
    {
        // Child process
        if (execvp(args[0], args) == -1) 
        {
            perror("tsh");
        }
        exit(EXIT_FAILURE);
    } 
    else if (pid < 0) 
    {
        // Error forking
        perror("tsh");
    } 
    else 
    {
        // Parent process
        do 
        {
            wpid = waitpid(pid, &status, WUNTRACED);
        } 
        while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }
    return 1;
}


//Function for executing commands
int tsh_execute(char **args)
{
    int i;

    if (args[0] == NULL) 
    {
        // An empty command was entered.
        return 1;
    }

    for (i = 0; i < tsh_num_builtins(); i++) 
    {
        if (strcmp(args[0], builtin_str[i]) == 0) 
        {
            return (*builtin_func[i])(args);
        }
    }
    return tsh_launch(args);
}


//Function to read a line
char *tsh_readline(void)
{
    int buffsize = TSH_RL_BUFFSIZE; //Set buffer size
    int position = 0;
    char *buffer = malloc(sizeof(char) * buffsize);
    int c;

    if (!buffer) 
    {
            fprintf(stderr, "tsh: allocation error\n");
        exit(EXIT_FAILURE);
    }

    while (1) 
    {
        // Read a character
        c = getchar();

        // If we hit EOF, replace it with a null character and return.
        if (c == EOF || c == '\n') 
        {
            buffer[position] = '\0';
            return buffer;
        } 
        else 
        {
            buffer[position] = c;
        }
        position++;

        // If we have exceeded the buffer, reallocate.
        if (position >= buffsize) 
        {
            buffsize += TSH_RL_BUFFSIZE;
            buffer = realloc(buffer, buffsize);
            if (!buffer) 
            {
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

    if (!tokens) 
    {
        fprintf(stderr, "tsh: allocation error\n");
        exit(EXIT_FAILURE);
    }

    //Tokenize the string using whitespace as delimiters (strtok library)
    token = strtok(line, TSH_TOK_DELIM);
    while (token != NULL) 
    {
        tokens[position] = token;
        position++;

        if (position >= buffsize) 
        {
            buffsize += TSH_TOK_BUFFSIZE;
            tokens = realloc(tokens, buffsize * sizeof(char*));
            if (!tokens) 
            {
                fprintf(stderr, "tsh: allocation error\n");
                exit(EXIT_FAILURE);
            }
        }

        token = strtok(NULL, TSH_TOK_DELIM);
    }
    tokens[position] = NULL;
    return tokens;
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


//Main loop
int main(int argc, char **argv)
{
    //Load config files here

    //Run main command loop
    tsh_loop();

    //Shutdown/cleanup.
    return EXIT_SUCCESS;
}
//EOF









