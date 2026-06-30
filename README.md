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


