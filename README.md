# MiniBox
A busybox-like/toybox-like lighweight replacement aiming to be lightweight and ultra-portably
memory efficient.

MiniBox - 

![MINIBOX-wTitle](https://github.com/Qwer-TeX/minibox/assets/128016252/df4b6b83-2adf-434a-9d46-a907a9641a51)

NOTE: 

 - No error checking
 - No debug
 - No real case uses (idk any - maybe embedded projects ? or learning C ???)

 - No useless symboles injected into objects
 - Only simplicity and low memory usage
 - Best of all, no bloat, anykind, source-code bloat, object bloat, etc.


  I know (maybe) that this project would not succeed against BusyBox or 
  ToyBox(as of right now). Almost same multi-call binary that you can't even show 
  the difference from, only which has the most arguments(-h, -v, etc).
  Maybe I'll introduce arguments that are most widely used in programs respectivly.

  Also see TOOLBOX-INTRO to learn more about the tools philosophy.

  It can be used for many embedded devices, like for example, cheap routers, 
  cheap modems which have a NVM, eMMC smaller than the size of 1.2MB, 720K, or 200K
  that are cheap to manufacture though there is no warranty like in busybox, 
  and toybox, but a guarantee that you can modify and distribute edited sources to fit
  your own needs, 
  it one of the reasons I made the code to this project novice understandable ;).

  Don't be surprised if you firmware extracted a linux embedded device and found MiniBox in there :D.

  I didn't split minibox.c into wc.c, cpcat.c, main.c, etc on purpose because once I did, I saw a 
  small size difference. I tried stripping it but it still would remain the same size. I tried 
  debugging and objdump'ing nm'ing it but it proved somewhat useful, it show an extra addition
  of a symbol, or something like that, that was just a pure annoyance. I from here on out, I didn't
  split the source of this project. Just like in the sqlite3 source code, they have 1 large source 
  which I can compile using gcc without its makefile, same here. The split source version of 
  minibox produced a executable 13K larger and 4K larger when stripped. I then diff'ed this split 
  source compiled executable with the original minibox executable and it turned out not to be the
  same when it should clearly be the same. Its a compiler issue that someone needs to address to 
  gcc. I haven't tried with clang but it could be the same. I haven't also tried with tcc either
  but I really should and I will.
