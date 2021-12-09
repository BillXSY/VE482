# VE482 — Introduction to Operating Systems Homework 8

518370910200 Shengyuan Xu



## Ex. 1 — Input/Output

1. According to the textbook, the speed of a 300dpi scanner is 1MB/s, and the wireless network (802.11n) runs at 37.5MB/s. So there's no problem at all.
2. UNIX does it as follows. There is a table indexed by device number, with each
   table entry being a C struct containing pointers to the functions for opening,
   closing, reading, and writing and a few other things from the device. To install
   a new device, a new entry has to be made in this table and the pointers filled
   in, often to the newly loaded device driver.
3. (a) Device driver.
   (b) Device driver.
   (c) Device-independent software.
   (d) User-level software.

## Ex. 2 — Multiprocessors



## Ex. 3 — File systems



## Ex. 4 — Security



## Ex. 5 — Research



## Ex. 6 — Linux



## Ex. 7 — Course survey

Done!!!



## Reference