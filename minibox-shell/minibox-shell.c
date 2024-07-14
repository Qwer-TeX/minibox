/* MiniBox is a busybox/toybox like replacement aiming to be lightweight, portable, and 
 * memory efficient.
 *
 * Copyleft (C) 2024-2024 Robert Johnson <mitnew842 AT gmail dot com>
 *
 * Licensed under GPLv2 or later, see file LICENSE in this source tree.
 * minibox-shell.c is the main shell source with all shell builtins.
 *
 * When adding programs or features, please consider if they can be
 * accomplished in a sane way with standard unix tools. If they're 
 * programs or features you added, please make sure they are read-
 * able and understandable by a novice programmer, if not, add 
 * comments or let me know.
 *
 * I haven't tested but it could compile on windows systems with MSYS/MinGW or Cygwin.
 * MiniBox should be fairly portable for POSIX systems.
 *
 * Licensed under GPLv2 or later, see file LICENSE in this source tree.
 */

/* XXX: Writing a shell is a very complex task, though I don't have time to write one.
 *      Contributions of a shell are welcome as they give me a half-complete structure
 *      to work on instead of deciding where to start. Or if someone could fork the ash
 *      sources from busybox so that we could try to simplify the source code resulting
 *      in a ash shell that I could implement, that would be excellent. We could create
 *      the very stripped-down version of the busybox ash which would come to be known 
 *      as the minibox shell. Compiling busybox with allnoconfig and menuconfig it to
 *      enable ash and its features would result in a 88k executable. It could become 
 *      probably 14k to 48k if we were to fork and strip it down ;). Or as usual, get 
 *      started at creating our own shell from scratch but that would be off the limits 
 *      for me since I don't have enough time to do it all though I can create examples
 *      since they're simple and effective.
 *
 *      The minibox shell has to be implemented in a seperate file and directory because 
 *      I know that the lines of code for this shell would be atleast 600 lines with 
 *      comments included. Remember its the stripped-down version of the busybox ash so 
 *      its has to not exceede 600-1000 lines of code. But the example shells are less 
 *      than 200 lines of code so I thought maybe we can make our shell ultra small but 
 *      remember those example shells are only stripped down and don't yet have 
 *      implemented its shell language functions like if, case, else, until, for, test, 
 *      etc. If someone can contribute codes for those functions, I would me much more 
 *      than thankful.
 *
 *      No its not going to be seperate from the minibox.c since minibox.o and minibox-
 *      shell.o are going to be linked together because this projects's goal is for a 
 *      multi-call binary but I'll give two options you can adjust in the Makefile for
 *      creating a standalone shell or including it in the multi-call binary.
 */
