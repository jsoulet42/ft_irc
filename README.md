Minishell Project
This repository contains my implementation of the Minishell project as part of my studies at Ecole 42.

Description
Minishell is a simple Unix shell emulator developed in C. It provides a basic command-line interface where users can execute various commands such as cd, echo, pwd, export, unset, and more.

The goal of this project is to understand and implement a small-scale shell program that handles basic command execution, environment variables, and signal handling. This implementation focuses on parsing commands, managing the environment, and executing them as separate processes.

Usage
To compile the Minishell program, run:

bash
Copy code
make
This will build the executable named minishell.

To run Minishell, execute the following command:

bash
Copy code
./minishell
Features
Basic command execution (ls, echo, cd, etc.)
Environment variables management (export, unset)
Signal handling
Usage Notes
The Minishell program mimics a basic shell environment; however, it may not cover the full range of features found in standard Unix shells.
For detailed information on supported commands and functionalities, refer to the source code and the project's documentation.
