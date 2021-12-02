# VE482 — Introduction to Operating Systems Lab 9

[TOC]

## Tasks

1. **What needs to be returned by read and write file operations for a character device?**

   The value returned by read or write can be:
   
   > - the number of bytes transferred; if the returned value is less than the size parameter (the number of bytes requested), then it means that a partial transfer was made. Most of the time, the user-space app calls the system call (read or write) function until the required data number is transferred.
   > - 0 to mark the end of the file in the case of read ; if write returns the value 0 then it means that no byte has been written and that no error has occurred; In this case, the user-space application retries the write call.
   > - a negative value indicating an error code.



2. **How are exactly those major and minor numbers working? You vaguely remember that you can display them using `ls -l /dev`.**

   **Major** and **minor** numbers are associated with the device special files in the `/dev` directory and are used by the operating system to determine the actual driver and device to be accessed by the user-level request for the special device file.

   ![img](http://osr600doc.sco.com/en/HDK_concepts/graphics/major.gif)

   In `/dev`, we can see the major and minor numbers of different devices

   ![major&minor](.\pics\major&minor.png)




3. **Knowing the major number and minor numbers of a device, how to add a character device to `/dev`?**

   ```shell
   # (Create a new device directly)
   mknod /dev/$device_name c $major_id $minor_id
   
   # (add a character device to /dev)
   cdev_add(struct cdev *dev, dev_t num, unsigned int count);
   ```

   

4. **Where are the following terms located in Linux source code?**   

   | term                | location |
   | ------------------- | -------- |
   | module_init         |          |
   | module_exit         |          |
   | printk              |          |
   | container_of        |          |
   | dev_t               |          |
   | MAJOR               |          |
   | MINOR               |          |
   | MKDEV               |          |
   | alloc_chrdev_region |          |
   | module_param        |          |
   | cdev_init           |          |
   | cdev_add            |          |
   | cdev_del            |          |
   | THIS_MODULE         |          |

    

5. **How to generate random numbers when working inside the Linux kernel? You think that a while back you read something about getting the current time.**

   

6. **How to define and specify module options?**

   

## Reference

[1] return value of read and write https://linux-kernel-labs.github.io/refs/heads/master/labs/device_drivers.html

[2] major and minor numbers http://osr600doc.sco.com/en/HDK_concepts/ddT_majmin.html
