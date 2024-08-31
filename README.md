# MiniBox
Ultra small and simple micro-embedded Linux command set

MiniBox - 
![MINIBOX-wTitle](https://github.com/user-attachments/assets/5e0caa54-876e-40a6-8fe3-186027ac4fa3)


This project is not a fork of any BusyBox or ToyBox, it is fully written from scratch.

# What is this project?

This project contains many ultra mini sized utilies made in C that compares to the full sized coretuils'
utilites. 1 major difference is that the mini sized utilites lack very uncommon options and prioritizes
size over features, they can be compiled standalone seperatly from MiniBox at your own will.

# Why MiniBox
Honestly, why would anyone use this?

  1. Very small, comes under 60K.

  2. Very fast, (7.3x faster than BusyBox) (3.1x ToyBox)

  3. Simple, easy to understand source (0 code bloat).

  4. Supports all libc's (dietlibc, uclibc, musl, glibc, etc.)

  5. 0 Dependencies (with the exception of libc.)

  6. All utilities can compile standalone* (see Standalone Compilation) .


# How To Build
Plain and simple:

  To build:

    1. ./configure (no options) OR if you want to compile all commands `yes | ./configure` would be fine

    2. make

    3. make install (Installs to install_dir)

  To remove individually compiled objects

    1. make clean

  To uninstall (you can't, because it's copied to install_dir, but something familiar):

    1. make clean


IMPORTANT:

  After running ./configure, if you ever need to edit any code, run one of these first!!

    1a. make clean

    OR

    1b. ./scripts/toggle_ifdef.sh -D (recommended)

  This would remove the ifdefs from the source files so that all compile without ./configure
  this is also respectively self-explanitory if you understand the purpose.


# Standalone Compilation
1. To compile a utilitiy standalone, it is very easy, first don't run `./configure` or `make`, if you already have, run
   one of these first, `make clean` or (recommended) `./scripts/toggle_ifdef.sh -D`, choose which one fits you best.

2. Second, go edit the respective source of the utility you want to compile, remove `#include "minibox.h"` and replace with
   its respective headers and function definitions (I recommend creating a copy of the utility source into your $HOME directory)
   or don't remove the header and go to step 3.

3. Last but not least, add the main function, it always will look like the one below:

    +---------------------------------------------------------------------------------------------+
    |   example.c:                                                                                |
    |                                                                                             |
    |   /~/~/~/~/~/~/~/~/~/~/~/~/~/~/ utility code in example() /~/~/~/~/~/~/~/~/~/~/~/~/~/~/~/~  |
    |                                                                                             |
    |   int main(int argc, char *argv[])                                                          |
    |   {                                                                                         |
    |       return example(argc, argv);                                                           |
    |   }                                                                                         |
    |   ~                                                                                         |
    |   ~                                                                                         |
    |   ~                                                                                         |
    |   ~                                                                                         |
    +---------------------------------------------------------------------------------------------|

4. If you ever see the define `VERSION` in any source code, respectively choose one of the following:
    
    4a. Add `#define VERSION <some random chars>` or include `-DVERSION=\"some rand chars\"`
    4b. Remove `VERSION` and the corresponding `%s` in the {f}printf statement as a whole and/or replace with your fitting.

5. Most of the time, programs use reusable functions from libmb, in that case, do the following:

    1. Find what function is from libmb
    2. The corresponding function would be stored in libmb/<function name>.c
    3. Copy the code for that function to the program you want to compile standalone
    4. If you can find more function from libmb in the program, do steps 1 - 3 again.

    OR

    1. Find what function is from libmb.
    2. Inspect the code for that function from libmb and try to create your own
    3. Create your own clone of that function from libmb into your program.
    4. Do the same again if you find more functions from libmb

6. Now compilation time

    `gcc -flto -o <utility> <utility>.c`, that was simple enough, you are not limited to add more compilation flags in,
    like from the Makefile `-Wall -Wextra` for extra warnings or `-g` to include debugging symbols, '-s' to not include 
    any debugging symbols, '-Oz' aggressive size optimization, `-O2` for performance optimization, etc.

7. (Optional) Static Standalone Compilation:

    To statically compile without depending on any libraries, repeat steps 1 - 4 from Standalone Compilation.

    `gcc -static -flto -o <utility> <utility>.c -lc`, if compilation fails, check to see what functions is the utility using
    and check from where is the function avaliable from a library, a math function belongs in libm, so include `-lm` or gcc
    could do that for you sometimes.


# Adding your own implementation of utilites
    If you want to contribute or add your very own utilities, then this section is for you.

    Adding in your own utilites is very simple, decide what program you want to add to MiniBox
    then do the following.

    1. To add the program, create its source file (example would be used from here on) (e.g. src/example.c)

    2.  Now edit it to include `#include "minibox.h"`, because all headers go into minibox.h
    2b. If your program consist of more than 1 function then `#include "utils.h"` and add its
        function definitions in utils.h because they can be used in other program too, if not,
        still just add them.

    3. Add the program's function defintions in minibox.h, an example is described below

        +---------------------------------------------------------------------------------------------+
        |   include/minibox.h:                                                                        |
        |                                                                                             |
        |   /~/~/~/~/~/~/~/~/~/~/~/~/~/~/ minibox.h /~/~/~/~/~/~/~/~/~/~/~/~/~/~/~/~                  |
        |                                                                                             |
        |   /* example program */                                                                     |
        |   int example(int argc, char *argv[]);                                                      |
        |                                                                                             |
        |   ~                                                                                         |
        |   ~                                                                                         |
        |   ~                                                                                         |
        |   ~                                                                                         |
        +---------------------------------------------------------------------------------------------+

    4. Create the function with the program name in src/example.c and code what it does.
       (NOTE that your program has to do manual option parsing not using getopt because of maintaining
       a small size, see other files to see how they do it, a option parser would be added in libmb
       in later versions)

    5. After you are done writing your program, in src/example.c edit src/minibox.c to update the commands
       table, the table is a struct described below:

        +-----------------------------------+
        |                                   |
        |   // Command struct               |
        |   typedef struct {                |
        |     const char *cmd_name;         |
        |     CommandFunc cmd_func;         |
        |   } Command;                      |
        |                                   |
        +-----------------------------------+

        and this is how you would append your program to the end of the commands table


        +---------------------------------------------------------------------------------------------+
        |   include/minibox.c:                                                                        |
        |                                                                                             |
        |   /~/~/~/~/~/~/~/~/~/~/~/~/~/~/ minibox.c /~/~/~/~/~/~/~/~/~/~/~/~/~/~/~/                   |
        |                                                                                             |
        |       Command commands[] = {                                                                |
        |       #ifdef CONFIG_WC                                                                      |
        |           {"wc", wc},                                                                       |
        |       #endif                                                                                |
        |       /~/~/~/~/~/~/~/~/~/~/~/~/~/~/~/~/~/~/~/~/~/~/~/~/~/~/~/~/~/~/~/                       |
        |       #ifdef CONFIG_EXAMPLE                                                                 |
        |           {"example", example},                                                             |
        |       #endif                                                                                |
        +---------------------------------------------------------------------------------------------+

        a.  `const char *cmd_name` would be how example() would be invoked, can be invoked with 
        b.  `./minibox example` or `./example` if example is a symlink to minibox
        c.  `CommandFunc cmd_func;` would be the function executed when minibox is invoked as described
            in subsection b

        The struct was only made to simplify and more modularize the code to make adding in more programs
        easier, before, it used the `else if !strcmp(cmd, "example")` which was too much and would cause
        more function overhead and increase the executable size.

    6. Add into Makefile
    
        Edit the makefile to include the `example` program into the PROGS variable.

        Then you are done, the ./configure and ./scripts/toggle_ifdef.sh would automatically do the job for you,
        no need to edit those unless you want to improve upon them.

    7. Test and execute

        Now you can select yes or press enter when running ./configure to example.
        Now you can compile and link that program into the minibox executable.

        Hope this helps :)
