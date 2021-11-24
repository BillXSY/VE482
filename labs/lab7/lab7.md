## VE482 — Introduction to Operating Systems Lab 7

**Group-pair 8**: 518370910200 Shengyuan Xu, 518370910216 Xiangjie Li

[TOC]

### 1 What is a kernel module, and how does it different from a regular library?

Kernel modules are pieces of code that can be loaded and unloaded into the kernel upon demand. They extend the functionality of the kernel without the need to reboot the system. A module can be configured as built-in or loadable. To dynamically load or remove a module, it has to be configured as a loadable module in the kernel configuration.

A shared library is another software module with these properties: The `.text` address space is accessible to all applications. The `.data` and `.bss` address space is in the same address space as the application that uses the shared library. So, they are shared in the sense that the `.text` is shared.



### 2 How to compile a kernel module?

1. edit Makefile:

   ```bash
   obj-m = foo.o
   KVERSION = $(shell uname -r)
   all:
           make -C /lib/modules/$(KVERSION)/build M=$(PWD) modules
   clean:
           make -C /lib/modules/$(KVERSION)/build M=$(PWD) clean
   ```

2. Compile module using make command:

   ```bash
   make
   ```

   

3. load it using `insmod` command. (root user is recommended)

   ```bash
   insmod foo.ko
   ```

   

### 3 Write and test all the commands that are only hinted in the README file. 

Done. Check the source code.



### 4 How are mutex defined and used? How good is this approach? 

Three mutexs are defined:

- `dadfs_sb_lock`: Used for locking super block when action is taken on block
- `dadfs_inodes_mgmt_lock`: Used for locking super block when block inode need to be changed
- `dadfs_directory_children_update_lock`: Used for locking super block when children of the block is changed

Benefits of this approach:

- without these, troubles are likely to occur during the operation of the filesystem.
- inhance the efficiency of the code.



### 5 How is information shared between the kernel and user spaces?

By use the kernel APIs for manipulating user memory, such as`copy_to_iter` and `copy_from_iter`. The source codes are as follows.

```c
// Copies a block of data from the kernel to user space
size_t copy_to_iter(const void *addr, size_t bytes, struct iov_iter *i)
{
	const char *from = addr;
	iterate_and_advance(i, bytes, v,
		__copy_to_user(v.iov_base, (from += v.iov_len) - v.iov_len,
			       v.iov_len),
		memcpy_to_page(v.bv_page, v.bv_offset,
			       (from += v.bv_len) - v.bv_len, v.bv_len),
		memcpy(v.iov_base, (from += v.iov_len) - v.iov_len, v.iov_len)
	)

	return bytes;
}
EXPORT_SYMBOL(copy_to_iter);

// Copies a block of data from user space to the kernel
size_t copy_from_iter(void *addr, size_t bytes, struct iov_iter *i)
{
	char *to = addr;
	iterate_and_advance(i, bytes, v,
		__copy_from_user((to += v.iov_len) - v.iov_len, v.iov_base,
				 v.iov_len),
		memcpy_from_page((to += v.bv_len) - v.bv_len, v.bv_page,
				 v.bv_offset, v.bv_len),
		memcpy((to += v.iov_len) - v.iov_len, v.iov_base, v.iov_len)
	)

	return bytes;
}
EXPORT_SYMBOL(copy_from_iter);
```



### 6 Change

```bash
17,20d16
< #include <linux/uio.h>
< #include <linux/kmod.h>
< #include <linux/blktrace_api.h>
< 
275d270
< #if LINUX_VERSION_CODE < KERNEL_VERSION(3,11,0)
278,280d272
< #else
< ssize_t dadfs_read(struct kiocb *kiocb, struct iov_iter *to)
< #endif
285,291d276
< #if LINUX_VERSION_CODE >= KERNEL_VERSION(3,11,0)
<     struct file* filp = kiocb->ki_filp;
<         loff_t *ppos = &(kiocb->ki_pos);
<         size_t len = iov_iter_count(to);
< #endif
< 
< 
316,321c301
< #if LINUX_VERSION_CODE < KERNEL_VERSION(3,11,0)
<     if (copy_to_user(buf, buffer, nbytes))
< #else
<     if(!copy_to_iter(buffer,len,to))
< #endif
<     {
---
>         if (copy_to_user(buf, buffer, nbytes)) {
375d354
< #if LINUX_VERSION_CODE < KERNEL_VERSION(3,11,0)
378,380d356
< #else
< ssize_t dadfs_write(struct kiocb* kiocb, struct iov_iter * from)
< #endif
396,403c372
< #if LINUX_VERSION_CODE >= KERNEL_VERSION(3,11,0)
<     struct file* filp = kiocb->ki_filp;
<         loff_t *ppos = &(kiocb->ki_pos);
<         size_t len = iov_iter_count(from);
< #endif
<     sb = filp->f_inode->i_sb;
< 
< 
---
>         sb = filp->f_path.dentry->d_inode->i_sb;
409,420c378,380
< 
< #if LINUX_VERSION_CODE < KERNEL_VERSION(3,11,0)
<     retval = generic_write_checks(filp, ppos, &len, 0);
< #else
<     retval = generic_write_checks(kiocb,from);
< #endif
< # if LINUX_VERSION_CODE < KERNEL_VERSION(3,11,0)
<     if (retval){
< #else
<     if(!retval){
< #endif
<         return retval;}
---
>         retval = generic_write_checks(filp, ppos, &len, 0);
>         if (retval)
>                 return retval;
435d394
< 
445,450c404,405
< #if LINUX_VERSION_CODE < KERNEL_VERSION(3,11,0)
<     if (copy_from_user(buffer, buf, len))
< #else
<     if (!copy_from_iter(buffer,len,from))
< #endif
<     {
---
> 
>         if (copy_from_user(buffer, buf, len)) {
495,502c450,451
< 
< #if LINUX_VERSION_CODE < KERNEL_VERSION(3,11,0)
<         .read = dadfs_read,
<             .write = dadfs_write,
< #else
<         .read_iter = dadfs_read,
<         .write_iter = dadfs_write,
< #endif
---
>         .read = dadfs_read,
>         .write = dadfs_write,
773c722,723
<         
---
>         printk(KERN_INFO "Journal device is: %s\n", __bdevname(dev, b));
> 
775d724
<         printk(KERN_INFO "Journal device is: %s\n", bdevname(bdev, b));
1039d987
< 
```

### 7 Test result

#### 0 install dependencies

Done.

#### 1 compile: make

![image-20211118221016603](/Users/billxsy/Library/Application Support/typora-user-images/image-20211118221016603.png)

#### 2 create a small virtual disk (to be formated in dadfs): dd bs=4096 count=100 if=/dev/zero of=disk

![middle_img_v2_cad06f37-cefa-437a-bbcf-e9ee9833d90g](/Users/billxsy/Library/Application Support/LarkShell/sdk_storage/6f26245d6381fc59044c21a6de3e60fe/resources/images/middle_img_v2_cad06f37-cefa-437a-bbcf-e9ee9833d90g.png)

#### 3 create a small virtual disk (to be used as dadfs' journal): dd bs=1M count=10 if=/dev/zero of=journal

![middle_img_v2_b91f418e-0d51-4f29-a89d-13f347efe26g](/Users/billxsy/Library/Application Support/LarkShell/sdk_storage/6f26245d6381fc59044c21a6de3e60fe/resources/images/middle_img_v2_b91f418e-0d51-4f29-a89d-13f347efe26g.png)

#### 4 initialise the journla: mke2fs -b 4096 -O journal_dev journal

![middle_img_v2_93b62d21-ef79-4c72-970d-c31c2e8de39g](/Users/billxsy/Library/Application Support/LarkShell/sdk_storage/6f26245d6381fc59044c21a6de3e60fe/resources/images/middle_img_v2_93b62d21-ef79-4c72-970d-c31c2e8de39g.png)

#### 5 format the disk: ./mkfs-dadfs disk

![middle_img_v2_ea66d16a-8a6a-4f22-9cd9-2c9c23fd317g](/Users/billxsy/Library/Application Support/LarkShell/sdk_storage/6f26245d6381fc59044c21a6de3e60fe/resources/images/middle_img_v2_ea66d16a-8a6a-4f22-9cd9-2c9c23fd317g.png)

#### 6 load dadfs module: insmod

![middle_img_v2_a65b6454-3925-4aa6-8dce-cad4869a9f5g](/Users/billxsy/Library/Application Support/LarkShell/sdk_storage/6f26245d6381fc59044c21a6de3e60fe/resources/images/middle_img_v2_a65b6454-3925-4aa6-8dce-cad4869a9f5g.png)

#### 7 mount disk: losetup, mount (loop,journal_path)

![middle_img_v2_6b2cf0b1-9435-458b-8188-957aa46fe72g](/Users/billxsy/Library/Application Support/LarkShell/sdk_storage/6f26245d6381fc59044c21a6de3e60fe/resources/images/middle_img_v2_6b2cf0b1-9435-458b-8188-957aa46fe72g.png)

![middle_img_v2_a35a2b45-d2ad-4224-a9c5-7dd5068655dg](/Users/billxsy/Library/Application Support/LarkShell/sdk_storage/6f26245d6381fc59044c21a6de3e60fe/resources/images/middle_img_v2_a35a2b45-d2ad-4224-a9c5-7dd5068655dg.png)

#### 8 play with dad filesystem: mkdir, mv, cp, cat, rm, ls, cd, touch, etc.

![middle_img_v2_653217f0-245e-474d-8a67-fcbf2e6e0adg](/Users/billxsy/Library/Application Support/LarkShell/sdk_storage/6f26245d6381fc59044c21a6de3e60fe/resources/images/middle_img_v2_653217f0-245e-474d-8a67-fcbf2e6e0adg.png)

#### 9 check the logs: /var/log, dmesg

![middle_img_v2_10200105-ef9e-4772-9fe7-f4db82a9532g](/Users/billxsy/Library/Application Support/LarkShell/sdk_storage/6f26245d6381fc59044c21a6de3e60fe/resources/images/middle_img_v2_10200105-ef9e-4772-9fe7-f4db82a9532g.png)

#### 10 umount disk: losetup, umount

Done.

#### 11 unload module: rmmod

Done.



