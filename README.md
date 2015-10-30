Systems Integration CA1: Build a Custom Shell for Linux.

Simple implementation of shell in C: read, parse, fork, exec and wait. 
Includes internal commands cd, help, exit, dt (date) and ud (user data) 
and external commands pwd (pd), ifconfig (ifc). This shell can be used 
as the default shell for a user upon login, by using the command 
"chsh /path/to/shell/file username" to change the default shell. 
Please make a new user before you change the default shell, 
as otherwise you will be stuck with the custom shell.

This shell has some limitations:

Commands must be on a single line.
Arguments must be separated by whitespace.
No quoting arguments or escaping whitespace.
No piping or redirection.
Internal commands are: cd, ls, help, exit, dt (date) and ud (user data), clear, more, less.
External commands are: pwd (pd), ifconfig (ifc).
ifc can be used with the following tails: lo, eth1, wlan1.


https://github.com/lauramcgov3/SysIntCA1