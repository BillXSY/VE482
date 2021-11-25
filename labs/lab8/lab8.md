# VE482 — Introduction to Operating Systems Lab 8

**Group-pair 8**: 518370910200 Shengyuan Xu, 518370910216 Xiangjie Li

## 2 Memory in Minix 3

### 2.1 Memory management at kernel level

- What does `vm  `stands for? (Hint: in this context the answer is not virtual machine)

  Virtual memory.

- Find the page table definition and search what fields each entry contain?

- What basic functions are used to handle virtual memory?、

- Find all the places where the `vm` used inside the kernel, Why does it appear in so many different places?

- How is memory allocated within the kernel? Why are not `malloc` and `calloc` used?

- While allocating memory, how does the functions in kernel space switch back and fro between user
  and kernel spaces? How is that boundary crossed? How good or bad it is to put vm in userspace?

- How are pagefaults handled?