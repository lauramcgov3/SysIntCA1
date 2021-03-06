/*

Author: Laura McGovern, C12431378
Assignment: Systems Integration, Assignment 1
Date: 26th October 2015
GitHub: https://github.com/lauramcgov3/SysIntCA1

*/

#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <grp.h>
#include <sys/types.h>
#include <libgen.h>
#include <sys/stat.h>
#include <dirent.h>
#include <pwd.h>
#include <errno.h>

//Global declarations
#define TSH_RL_BUFFSIZE 1024 //Buffer size for reading a line
#define TSH_TOK_BUFFSIZE 64


//Function declarations for commands 
int tsh_cd(char **args);
int tsh_dt(char **args);
int tsh_ud(char **args);
int tsh_ls(char **args);
int tsh_help(char **args);
int tsh_clear(char **args);
int tsh_exit(char **args);
int tsh_ifc(char **args);
int tsh_pw(char **args);
int tsh_mkd(char **args);
int tsh_rmd(char **args);

//List the commands, 
//followed by a list of the functions to go with those commands.
char *builtin_str[] = 
{
    "cd",
    "dt",
    "ud",
    "ls",
    "help",
    "clear",
    "exit",
    "mkd",
    "rmd",
    "ifc",
    "pw"
};

int (*builtin_func[]) (char **) = 
{
    &tsh_cd,
    &tsh_dt,
    &tsh_ud,
    &tsh_ls,
    &tsh_help,
    &tsh_clear,
    &tsh_exit,
    &tsh_mkd,
    &tsh_rmd,
    &tsh_ifc,
    &tsh_pw,
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


//dt function
int tsh_dt (char **args)
{
    char buff[100];
    time_t now = time (0);
    strftime (buff, 100, "%Y%m%d%H%M%S", localtime (&now));
    printf ("%s\n", buff);
    return 1;
    
}

int tsh_ls (char **args)
{
    FILE *fptr;
    char line[130]; /* line from unix command*/

    fptr = popen("ls", "r"); /* Issue the ls command. */

    /* Read a line */
    while ( fgets( line, sizeof line, fptr))
    {
    printf("%s", line);

    /* or process the 'line' as needed */
    }
    pclose(fptr);
    return 1;
}

//ud function
int tsh_ud (char **args)
{
    
   //Get user ID
    int uid = getuid();
    
    //Get group ID
    int gid = getgid();
    
    //Get current username
    char *usrnm;
    usrnm=(char *)malloc(10*sizeof(char));
    usrnm=getlogin();
    
    //Get group name
    char* grpname;
	struct group* g;
    char** p;

    if( ( g = getgrgid( getgid() ) ) == NULL ) {
      fprintf( stderr, "getgrgid: NULL pointer\n" );
      return( EXIT_FAILURE );
    }
    grpname = g->gr_name;
    for( p = g->gr_mem; *p != NULL; p++ ) {
      printf( "\t%s\n", *p );
    }
    
    //get iNode
    DIR *dir;
    struct dirent *dp;
    int inode;
    if((dir = opendir("/home")) == NULL){
        printf ("Cannot open /home");
        exit(1);  
    }
    if ((dp = readdir(dir)) != NULL) {
        inode = dp->d_ino;
    }
    closedir(dir);
    
    
    //Print everything
    printf("%i, %i, %s, %s, %i. \n", uid, gid, usrnm, grpname, inode);
	
	return 1;
}


//Help function
int tsh_help(char **args)
{
    int i;
    printf("Laura McGovern's custom shell \n");
    printf("Type program names and arguments, and hit enter.\n");
    printf("The following are executable:\n");

    for (i = 0; i < tsh_num_builtins(); i++) 
    {
        printf("  %s\n", builtin_str[i]);
    }

    printf("Use the man command for information on other programs.\n");
    return 1;
}

//clear function
int tsh_clear(char **args)
{
    if (strcmp(args[0], "clear") ==0)
    {
        system("clear");
    }
    else
    {
        printf("Command not recognised. \n");
    }
    //free(args[0]);
    return 1;
}

//Exit function
int tsh_exit(char **args)
{
    return 0;
}

//mkd function
int tsh_mkd(char **args)
{
    
    
    //Get the name of the new directory
    char *dir;
    dir = args[1];
    //printf("%s \n", dir);
    
    //Get current working directory
    char path[1024];
    getcwd(path, sizeof(path));
    //printf("%s \n", path);
    
    //Combine path with new directory to get path
    strcat(path, dir);
    //printf("%s \n", path);
    
    struct stat st = {0};

    if (stat(path, &st) == -1) 
    {
        printf("%s created.\n", path);
        mkdir(path, 0755);   
    }
    else
    {
        printf("This directory already exists. \n");
    }
    
    return 1;
    
}

//rmd function
int tsh_rmd(char **args)
{

    char *dpath;
    
    //Get the name of the directory to delete
    char *dir;
    dir = args[1];
    printf("%s \n", dir);
    
    //Get current working directory
    char path[1024];
    getcwd(path, sizeof(path));
    printf("%s \n", path);
    
    //Combine path with new directory to get path
    strcat(path, dir);
    printf("%s \n", path);
    
    dpath = path;

    DIR *d = opendir(path);
    size_t path_len = strlen(path);
    int r = -1;

    if (d)
    {
      struct dirent *p;

      r = 0;

      while (!r && (p=readdir(d)))
      {
          int r2 = -1;
          char *buf;
          size_t len;

          /* Skip the names "." and ".." as we don't want to recurse on them. */
          if (!strcmp(p->d_name, ".") || !strcmp(p->d_name, ".."))
          {
             continue;
          }

          len = path_len + strlen(p->d_name) + 2; 
          buf = malloc(len);

          if (buf)
          {
             struct stat statbuf;

             snprintf(buf, len, "%s/%s", path, p->d_name);

             if (!stat(buf, &statbuf))
             {
                if (S_ISDIR(statbuf.st_mode))
                {
                   r2 = rmdir(buf);
                }
                else
                {
                   r2 = unlink(buf);
                }
             }

             free(buf);
          }

          r = r2;
      }

      closedir(d);
    }

    if (!r)
    {
      printf("%s removed \n", dpath);
      r = rmdir(dpath);

    }

   return 1;
}

//External command functions

//ifc function
int tsh_ifc(char **args)
{
    FILE *fp;
  char returnData[64];
  if (args[1] == NULL)
  {
	fp = popen("/sbin/ifconfig eth0","r");
  }
  else if(strcmp(args[1],"eth1")== 0 )
  {
	fp = popen("/sbin/ifconfig eth1","r");
  }
  else if (strcmp(args[1],"lo")== 0 )
  {
    fp = popen("/sbin/ifconfig lo","r");
  }
else if (strcmp(args[1],"wlan1")== 0 )
  {
    fp = popen("/sbin/ifconfig wlan1","r");
  }
  else{
	printf("Error in Syntax \n");
	return 1;
  }

  while(fgets(returnData,64,fp) !=NULL){
 	printf("%s",returnData);
  }
  pclose(fp);
  return 1;
}

//pw function
int tsh_pw(char **args)
{
    char cwd[1024];
    
    if(getcwd(cwd, sizeof(cwd)) != NULL)
    {
        fprintf(stdout, "The current directory: %s \n", cwd);
    }
    else
    {
        perror("getcwd() error");
    }
    return 1;
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

    int buffsize = TSH_TOK_BUFFSIZE, position = 0;
    char **tokens = malloc(buffsize * sizeof(char*));
    const char s[2] = " ";
    char *token;

    if (!tokens) 
    {
        fprintf(stderr, "tsh: allocation error\n");
        exit(EXIT_FAILURE);
    }

    //Tokenize the string using whitespace as delimiters (strtok library)
    token = strtok(line, s);
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

        token = strtok(NULL, s);
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
        //Get current username
        char *usrnm;
        usrnm=(char *)malloc(10*sizeof(char));
        usrnm=getlogin();
        printf("%s@customshell: ", usrnm); 
        line = tsh_readline(); //Calls function to read the line
        args = tsh_splitline(line); //Calls function to split line into arguments
        status = tsh_execute(args); //Execute arguements 

        // Free line and args
        free(line);
        free(args);
    }
    while(status); // Checking status variable
}

int info ()
{
    {
        printf("\n\n");
        FILE *file = fopen( "info.txt", "r" );

        /* fopen returns 0, the NULL pointer, on failure */
        if ( file == 0 )
        {
            printf( "Could not open file.\n" );
        }
        else
        {
            int x;
            /* Read one character at a time from file, stopping at EOF, which
               indicates the end of the file. Note that the idiom of "assign
               to a variable, check the value" used below works because
               the assignment statement evaluates to the value assigned. */
            while  ( ( x = fgetc( file ) ) != EOF )
            {
                printf( "%c", x );
            }
            fclose( file );
        }
    }
    return 1;
}

//Main loop
int main(int argc, char **argv)
{
    //Load config files here

    //Run main command loop
    info();
    tsh_loop();

    //Shutdown/cleanup.
    return EXIT_SUCCESS;
}
//EOF









