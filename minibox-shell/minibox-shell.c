/* MiniBox is a busybox/toybox like replacement aiming to be lightweight, portable, and 
 * memory efficient.
 *
 * Copyleft (C) 2024-2024 Robert Johnson <mitnew842 AT gmail dot com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *
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
 *      for me since I don't have enough time to do it all.
 *
 *      The minibox shell has to be implemented in a seperate file and directory because 
 *      I know that the lines of code for this shell would be atleast 6100 lines with 
 *      comments included. Remember its the stripped-down version of the busybox ash so 
 *      its has to not exceede 6100-7000 lines of code. I know, that's alot thats why I 
 *      didn't want to make one from scratch.
 *
 *      No its not going to be seperate from the minibox.c since minibox.o and minibox-
 *      shell.o are going to be linked together.
 */
