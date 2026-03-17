# MiniShell

# Mini Shell (Linux)

## Description
This project is a Mini Shell implemented in C. It behaves like a basic Linux shell where the user can execute commands.

## Features
- Internal Commands (cd, pwd, exit, jobs, fg, bg)
- External Commands (ls, cat, etc.)
- Pipe Support (|)
- Signal Handling (Ctrl+C, Ctrl+Z)
- Job Control

## Technologies Used
- C Programming
- Linux System Calls

## System Calls Used
- fork()
- execvp()
- waitpid()
- pipe()
- dup2()
- signal()

## How to Compile
gcc *.c -o minishell

## How to Run
./minishell

## Sample Commands
ls  
pwd  
cd /home  
ls | wc  
sleep 10  
jobs  
fg  
bg  

