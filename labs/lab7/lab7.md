VE482 — Introduction to Operating Systems Lab 7

518370910200 Shengyuan Xu

1. **What is a kernel module, and how does it different from a regular library?**

   Kernel modules are pieces of code that can be loaded and unloaded into the kernel upon demand. They extend the functionality of the kernel without the need to reboot the system. A module can be configured as built-in or loadable. To dynamically load or remove a module, it has to be configured as a loadable module in the kernel configuration.

   A shared library is another software module with these properties: The `.text` address space is accessible to all applications. The `.data` and `.bss` address space is in the same address space as the application that uses the shared library. So, they are shared in the sense that the `.text` is shared.

2. **How to compile a kernel module?**

   

3. 