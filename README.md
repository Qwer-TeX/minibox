# MiniBox
A ultra small and simple micro-embedded Linux command set

MiniBox - 

![MINIBOX-wTitle](https://github.com/Qwer-TeX/minibox/assets/128016252/df4b6b83-2adf-434a-9d46-a907a9641a51)

This projects aims to be small and efficient, enough to be suitable for micro-embedded systems or just a 
busybox or toybox replacement, though this project lacks (many) options (-l, -v, -n) its core functionality
(without options) is compatible with other commands and not for some.

This project is simple and self-explanitory enough that you can implement your own command set!

Use `Make -j` not `-j2` or `j16` since `-j` is the fastest I've seen with this project and doesn't limit
how many jobs are made.
