# blazersh

This is the beginning of the `blazersh` package for UAB's CS433 Operating Systems course. It is a basic shell.


## Installation

This project depends on the `check` library (installation instructions <a href="https://libcheck.github.io/check/web/install.html">here</a>).

Build with
- `./configure`
- `make`

Optionally run tests with
- `make check`


## Howto
execute with
- `./src/blazersh`

The `help` command reads as follows:
```
== Commands ==
environ: list environment variables
get <var>: show value for 'var' environment variable
set <var> <val>: set 'var' environment variable to 'val'
list: list files and subdirectories of the current directory
ls: alias of list
pwd: prints the present working directory
cd <dir>: change the present working directory to 'dir'
help: Show this text
If you have any problems, please report them to noreply@uab.edu
```