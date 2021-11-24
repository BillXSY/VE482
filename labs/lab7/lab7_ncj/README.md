# VE482 Lab 7 Changelog

Chujie Ni 518370910084 2020.11.17

## Overview

In order to fix and test the `dadfs`, I first read through the lab manual and get some useful hint:

* Look at source code of `linux/fs.h`, `linux/uio.h` on https://elixir.bootlin.com/linux
* Search for `struct kiocb`, `struct iov_iter`, `copy_to_iter`, `copy_from_iter` on that website and learn how to use them.
* Try to compile `dadfs` on Ubuntu20.04, and failed.
* Search for `generic_write_checks`, and find its latest version of API becomes `ssize_t generic_write_checks(struct kiocb *, struct iov_iter *);`
* Therefore, we need to rewrite the read and write operation of this file system in a style of `iov_iter`.
* By checking `linux/fs.h`, we find that in the struct `file_operations`, there are exactly two operations: `read_iter` and `write_iter`, that have the same type signature as expected.
* We then rewrite `dadfs_read` and `dadfs_write` to fit the new kernel API, and compile and test.

## File Changes

I only changed one file `base.c`, and the modifications can be separated into four parts.

### Header Files

We include `uio.h` in order to use `struct kiocb`, `struct iov_iter` and corresponding functions.

<img src="./img/code1.png" height="80%" width="80%"/>

### dadfs_read

We mainly modify the type signature of function, declare new variables, and use `copy_to_iter` to read data. We use macro to deal with different kernel version.

<img src="./img/code2.png" height="80%" width="80%"/>

<img src="./img/code3.png" height="80%" width="80%"/>

### dadfs_write

We mainly modify the type signature of function, declare new variables, and use `copy_from_iter` to write data. 

<img src="./img/code4.png" height="80%" width="80%"/>

<img src="./img/code5.png" height="80%" width="80%"/>

<img src="./img/code6.png" height="80%" width="80%"/>

<img src="./img/code7.png" height="80%" width="80%"/>

### File Operation Registration

Finally, we register different file operations according to kernel version.

<img src="./img/code8.png" height="80%" width="80%"/>

## Compile & Test

### Make

We first make a new dir called `mount` to mount our file system later. Then we go into the `dadfs` directory and make.

<img src="./img/cmd1.png" height="80%" width="80%"/>

### Virtual Disk

We then create a virtual data disk and a journal disk.

<img src="./img/cmd2.png" height="80%" width="80%"/>

### Initialize Journal Disk

We then initialize the journal disk to be used by the file system.

<img src="./img/cmd3.png" height="80%" width="80%"/>

### Make File System

Write the disk to be the format of `dadfs`.

<img src="./img/cmd4.png" height="80%" width="80%"/>

### Insert Module

<img src="./img/cmd5.png" height="80%" width="80%"/>

### Mount Journal

<img src="./img/cmd6.png" height="80%" width="80%"/>

### Mount Dadfs

<img src="./img/cmd7.png" height="80%" width="80%"/>

### Play With Dadfs

<img src="./img/cmd8.png" height="80%" width="80%"/>

<img src="./img/cmd9.png" height="80%" width="80%"/>

<img src="./img/cmd10.png" height="80%" width="80%"/>

### losetup

<img src="./img/cmd11.png" height="80%" width="80%"/>

### dmesg

<img src="./img/cmd12.png" height="80%" width="80%"/>

### Remove

Use `losetup -f /dev/xxx` to remove loop device. Use `umount` to unmount file system. Use `rmmod` to remove kernel module.

## References

[1] https://tldp.org/LDP/lkmpg/2.6/html/index.html