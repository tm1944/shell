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

Jul 1,2026
 - fork() => used to make a child process, this splits the program where the fork() was called. int     returns a 0 pid for the child process and a pid > 0 for the parent process. This process will
    then execute the rest of the code (copied from the parent) then die when it hits the return 0 in    main. However we are using execvp() which changes the program image to the file in first param.
- execvp() => We sacrifice our child program to run the file this points to. The second param is 
    the argsv is a vector of char* "C strings". This is what will be passed to the programs main 
    argv[0]. For example if file = "ls" and argsv = ["ls","-la",NULL] execvp will find the bin/ls 
    and then pass the argsv to the main function.
-waitpid() => This is for the parent function, as we wait for the child process to preform the 
    execvp() and die, the parent process waits. we use waitpid because we can select which pid_t        to wait for, and also we can collect the status of how the child died via &status. This is what 
    I am going to use to show error for that command no existing, hypothesis is that the status 
    would return -1, when execvp doesn't find the command that i inputed.
        - The hypothesis was correct we can use WIFEXITED() and WIFSIGNALED() to extrate the code of            status and see if it crashed or the file wasn't found. WEXITSATUS() to get the 8 bit 
            number to see the code! 
Memory BUGS:
make_argv() -> has a memory bug. Right now I am making a vector of char*, vector<string> tokens params get's passed by value and then I run .c_str() on each index of the string vector. This gives me a char* to the string internal buffer, when i return args all local vars are destroyed however my char* still point to the addresses, which now don't have the string values.
Solution => pass my vector<string> tokens by reference so that they are alive when exec_vp() run

MileStone 2 is now done, I have a very decent understanding of how this all works. I used AI to judge my design decisions and it really helped me understand why stacking thinks like WIFEXITED and WIFESIGNALED is bad and cleared allot more understanding wise for me.

*MileStone 3*
Implement built-in pwd cd and exit

*Notes*
For pwd i will be using getcwd() and MAXPATHLEN. I am relying allot more on the man pages to code,
rather then searching up things. Will see how it pans out. So I made a buffer with its size being the MAXPATHLEN, then i passed that and its size to getcwd() which returned a char* buffer which i printed.

For cd what we want is three commands:
cd alone -> got to the home dir using getenv (DONE)
cd <path> -> call chdir via tokens[1]
chdir fails -> perror

the combination of getenv and chdir makes builtin cd command very easy

*MileStone 4*
