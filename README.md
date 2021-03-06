
Systems Integration CA1: Build a Custom Shell for Linux.

Simple implementation of shell in C: read, parse, fork, exec and wait. 
Includes internal commands dt (date), ud (user data), ls, help, mkd (mkdir),
rmd(rm), clear and external commands pwd (pd), ifconfig (ifc). This 
shell can be used as the default shell for a user upon login, by using 
the command "chsh /path/to/shell/file username" to change the default shell. 
Please make a new user before you change the default shell, 
as otherwise you will be stuck with the custom shell.

Installation
Download or clone the source code from https://github.com/lauramcgov3/SysIntCA1.
Navigate into the folder via the command line.
Run the command 'gcc -o shell shell.c' to compile the c source code.
Run the command './shell' in order to enter the custom shell.

This shell has some limitations:

Commands must be on a single line.
Arguments must be separated by whitespace.
No quoting arguments or escaping whitespace.
No piping or redirection.


Internal commands are: dt (date), ud (user data), ls, help, mkd (mkdir),
rmd(rm), clear
External commands are: pwd (pd), ifconfig (ifc).
ifc can be used with the following tails: lo, eth1, wlan1.
mkd must be entered in the following format "mkd /newdir" in order to create a folder.
rmd must be entered in the format "rmd /dir".