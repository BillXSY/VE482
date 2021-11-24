# VE482 Lab 7

Chujie Ni 518370910084 2020.11.17

## 1. What is a kernel module?

Kernel modules are pieces of code that can be loaded and unloaded into the kernel upon demand. They extend the functionality of the kernel without the need to reboot the system. While for a regular library, you need to recompile it and reboot the system (similar as we are recompiling minix kernel).

## 2. How to compile a kernel module?

Linux use a technology called `kbuild` to build kernel modules. Below is a typical Makefile for a module source file, for example `hello.c`. Then you just type `make` in the current file to compile the module.

```makefile
obj-m += hello.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
```

## 3. Write and Test

For detail please see `README.md`.

## 4. Mutex

`mutex` are defined in at the beginning of `base.c`:

```c
static DEFINE_MUTEX(dadfs_sb_lock);
static DEFINE_MUTEX(dadfs_inodes_mgmt_lock);
static DEFINE_MUTEX(dadfs_directory_children_update_lock);
```

And are used with two functions:

```c
int mutex_lock_interruptible(struct mutex * lock);
void mutex_unlock(struct mutex * lock);
```

This strategy differs from common cases because it uses `mutex_lock_interruptible` instead of `mutex_lock`. In this case, if the process is sleeping and waiting for the lock, its sleep can be interrupted by signals sent by the system. If it cannot be interrupted, and since it is a kernel module, it is likely to trap the kernel and stuck the whole system. In this view, the design is good.

However, the first two mutex are good since super block and inode infomation is quite important. However, for `dadfs_directory_children_update_lock`, it won't allow creating new children in two different dirs at a time. There should be better design allowing this.

## 5. Information Sharing

For older kernel version, use `copy_from_user` and `copy_to_user`. For newer kernel version, use `copy_from_iter` and `copy_to_iter`. These functions allow you to copy data between user and kernel space.

## 6. Changes

For detail please see `README.md`.

## References

[1] https://tldp.org/LDP/lkmpg/2.6/html/index.html