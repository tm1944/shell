# shell

A Unix shell built from scratch in C++20 as a self-directed learning project. Covers the core primitives of process management on Linux (`fork`, `execvp`, `waitpid`, `dup2`, and `pipe`) by building up from a basic REPL loop to a shell that can execute real programs, handle built-in commands, and support I/O redirection and pipes. The goal isn't a feature-complete shell, but to build a working mental model of how a shell mediates between the user and the operating system.
