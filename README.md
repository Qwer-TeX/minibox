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
 - Almost little-to-none command line options


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

  I'd be surprised if I found MiniBox in a embedded system.
