# blazersh

A shell implemented in C supporting quoting, escaping, and file redirection. 
Part of UAB's CS-433 Operating Systems course. 

Author: Caleb Falcione - calebf@uab.edu


## Getting Started

This project depends on the `check` library 
(installation instructions <a href="https://libcheck.github.io/check/web/install.html">here</a>).

Build environment with
- `./configure`

Rebuild and compile with
- `make`

Optionally run tests with
- `make check`

After compiling, execute with
- `./src/blazersh`

## Testing

This project uses unit testing via the `check` library. To run these tests, run `make check`.
These tests can be viewed in tests/check_*.c

There are no integration tests or unit tests that directly check stdio or error 
handling. As a result, all of that code was lovingly written and checked by hand
with cases like the following:
```
$ ./src/blazersh 

blazersh>help
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

blazersh>help > help.txt

blazersh>cat help.txt
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

blazersh>rm help.txt

blazersh>sudo touch help.txt
[sudo] password for caleb:             

blazersh>help > help.txt
access denied

blazersh>sudo rm help.txt

blazersh>grep -i "AC" < configure.ac > filtered.txt

blazersh>cat filtered.txt
AC_PREREQ([2.69])
AC_INIT([blazersh], [0.1], [noreply@uab.edu])
AC_CONFIG_SRCDIR([config.h.in])
AC_CONFIG_HEADERS([config.h])
AC_PROG_AWK
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_LIBTOOL
AC_CHECK_HEADERS([stdint.h stdlib.h])
# Checks for typedefs, structures, and compiler characteristics.
AC_CONFIG_FILES([Makefile
AC_OUTPUT

blazersh>pwd
/home/caleb/Dropbox/school/cs433/blazersh

blazersh>cd src
/home/caleb/Dropbox/school/cs433/blazersh/src

blazersh>cd ..
/home/caleb/Dropbox/school/cs433/blazersh

blazersh>set PWD nonsense
nonsense

blazersh>cd src
/home/caleb/Dropbox/school/cs433/blazersh/src

blazersh>get PWD
/home/caleb/Dropbox/school/cs433/blazersh/src

blazersh>mkdir "test directory"

blazersh>rm test\ directory
rm: cannot remove 'test directory': Is a directory

blazersh>rm -r test\ directory

blazersh>exit
```