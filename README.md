**Objective**
Build a working command-line shell from scratch that can run real programs, handle built-in command,and support I/O redirection and pipes. The goal isn't a feature-complete shell but to build a working mental model of processes, file descriptors, and how a shell actually works!

**Out of Scope (For V1)**
- Scripting support
- Tab Completion or command history 
- Globbing (*.txt)
- Full Posix compliance 

**Milestones**
Recording what the goal for said day is and what was achieved/learned.

*Milestone 1*
Read a line from stdin in a loop, tokenize it on whitespace, print the tokens back.

Criteria
- Typing ls -la /tmp should print out [ls] [-la] [/tmp]
- Loop exits on typing exit     
- Handle empty inputs w/o crashing

Notes
stringToTokens, print back tokenized version of line
    - This takes a string and returns to a vector<string> using isstringstream, " " as delimeter
    - We are getting the whole line as a string using getline() (delimeter "\n")
    - getline() returns a reference to the isstream& (this can be used as true/false)
    - the string is then added to the second param variable
    - for now just printing back the line from the returned vector

*Milestone 2*
Use fork() + execvp() + waitpid() to run the parsed commands as a real programs. Also slight frontend improvement add a tm@shell $  or something at the beginning of each line.

Criteria
- ls, pwd , echo hello run and produce real output 
- If command doesn't exist print error instead of crashing
- Parent process waits for child before printing next prompt
- understand the fork,execvp and waitpid in depth

Notes
fork() - this creates a child process however doesn't run the process. It returns the child  PID on creation and -1 if the creation of the child fails.
execvp() -  #include <unistd.h> used to exec a executable file via path. It achieves this via replacing the current process with a new process loaded from executable file. We will be using this to exec the forked child process. 
    - int execvp(const char *file, char *const argv[]);
        - file => what program to run (name or path of the exec)
        - argv => argument vector for the new program
            - this an array of strings passed to the new program, this is the same one that main recieves (mental model wise not the literal same). argv must be NULL-terminated 
waitpid() => lets the parent wait for a specific child to finish ahd read how it exited
    - #include <sys/wait.h> 
    - pid_t waitpid(pid_t pid,int *status, int options);
    - pid > 0 means wait for the child with that PID, -1 means wait for any child, 0 means wait for any child in the same process group as the caller,
        < -1 means wait for any child whos process group ID equals -pid
    - status - where exit info is store, pass NULL if I don't care how the child exited
    - options - blocking behaviour 
    - We want to use waitpid() instead of wait because wait() waits for any child where waitpid() allows me to specify
FULL PATTERN => fork() -> exec -> waitpid
mental model => fork() creates child; parents get PID. execvp() child runs another program. waitpid() allows parent to pause until that child is done, then learns how it ended(exit or success)


TODO: 
    commands not working witout flags
    write up understanding of how this system works
THE GOOD:
    Got the shell to work somewhat for today, great first day of CPP. Only bout 400 good days for a     systems engineer job :D

