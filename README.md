# minishell_42

### The objective of this project is create a simple shell. A program can do a lexer, parser and executor.

<p align="center"> 
  <img src="https://i.imgur.com/5UF1zps.png" width="100%" />
</p>

### [TODO](#TODO)

* Show a prompt when waiting for a new command
* launch the executables based on the ``PATH`` or by using relative or absolute path like ``ls`` or ``/bin/ls``
* Implementation of the builtins:
    - [echo](#echo) with optin `` -n ``
    - [cd](#cd) with only a relative or absolute path
    - [pwd](#pwd) without any options
    - [export](#export) without any options
    - [unset](#unset) without any options
    - [env](#env) without any options or any arguments
    - [exit](#exit) without any options
* Separate the commands with semicolon `` ; ``
* Simple and double quotes work like in bash
* Redirections `` > `` , `` < `` and `` >> ``
* Pipes `` | ``
* Environment variables ( $"Name of variable" )
* Return value should work like bash ``$?``
* ``Ctrl-D`` and Signals ``Ctrl-C`` and ``Ctrl-\``
* Wilcard ( * ) work like in bash.

#### [External functions]()
```
printf, malloc, free, write, open, read, close, fork, wait, waitpid, wait3, wait4, signal, kill, exit, getcwd, chdir, stat, lstat, fstat, execve, dup, dup2, pipe, opendir, readdir, closedir, strerror, errno
```

### [How to lunch ?]()

> ```make```  will create an executable file with ``minishell`` as name

> ``./minishell`` will lunch the executable, and show the prompt






#### The project created by [abel-mak](https://github.com/abel-mak) && [a-saadi](https://github.com/a-saadi)