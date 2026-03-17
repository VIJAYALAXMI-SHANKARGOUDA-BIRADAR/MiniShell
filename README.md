# MiniShell

## 🧠 Project Overview

This project is a Mini Shell developed in C on Linux.  
It simulates basic functionalities of a Unix shell like Bash.

It allows users to enter commands, which are processed and executed using Linux system calls.

---

## ⚙️ Key Features

- Custom shell prompt (PS1 support)
- Internal commands (cd, pwd, exit, jobs, fg, bg)
- External commands (ls, cat, etc.)
- Pipe support (|)
- Signal handling (Ctrl+C, Ctrl+Z)
- Job control using linked list

---

## 🧩 Block Diagram

```
+-------------------+
|   User Input      |
|  (Keyboard)       |
+---------+---------+
          |
          v
+-------------------+
|   Mini Shell      |
|  (scan_input)     |
+---------+---------+
          |
          v
+-------------------+
| Command Parsing   |
| (get_command)     |
+---------+---------+
          |
          v
+-----------------------------+
| Command Type Identification |
| Built-in / External / Error |
+---------+-------------------+
          |
   -------------------------
   |                       |
   v                       v
+-----------+       +----------------+
| Internal  |       | External       |
| Commands  |       | Commands       |
+-----------+       +----------------+
     |                     |
     v                     v
+-----------+       +----------------------+
| Execute   |       | fork()               |
| Directly  |       | execvp()             |
+-----------+       | waitpid()            |
                    +----------+-----------+
                               |
                               v
                      +------------------+
                      |   Output to User |
                      +------------------+
```

---

## 🛠️ Technologies Used

- C Programming
- Linux System Calls

---

## ⚡ System Calls Used

- fork()
- execvp()
- waitpid()
- pipe()
- dup2()
- signal()

---

## ▶️ How to Compile

```
gcc *.c -o minishell
```

---

## ▶️ How to Run

```
./minishell
```

---

## 💻 Sample Commands

```
ls
pwd
cd /home
ls | wc
sleep 10
jobs
fg
bg
```

---

