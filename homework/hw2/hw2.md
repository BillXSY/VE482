# VE482 -- Intro. to OS Homework 2

Shengyuan Xu 518370910200

[TOC]

## EX. 1 — Multiprogramming

1. Probability: $p^n$

   CPU utilisation: $1-p^n$

2. ![untitled](/Users/billxsy/Desktop/2021FA/VE482/homework/hw2/untitled.jpg)

3.  

   a) $\frac{256 - 96}{48} = 3.33$, therefore **three** process can be stored simultaneously in memory

   b) $CPU\ utilisation = 1 - 0.9^3 = 0.271$

   c) adding 256MB:   $CPU\ utilisation = 1 - 0.9^{floor(\frac{256+256 - 96}{48})} = 0.570$. Increase: 0.117%/MB.

   ​    adding 512MB:   $CPU\ utilisation = 1 - 0.9^{floor(\frac{512+256 - 96}{48})} = 0.771$. Increase: 0.098%/MB.

   ​    adding 1024MB: $CPU\ utilisation = 1 - 0.9^{floor(\frac{1024+256 - 96}{48})} = 0.920$. Increase: 0.063%/MB.

   ​	Therefore, adding a 256MB RAM is the most beneficial and would be worth the incestment.



## Ex. 2 — Keymap in Minix 3

By searching, I find that:

- the keys are defined in `/usr/src/services/is/dmp.c`
- the functions related to the keys are in `.../kernel_dmp.c`
- the function prototypes are in `.../proto.h`
- after modifying those files, we need to `make build` in `/usr/src` then `reboot`.



In `/usr/src/servers/is/dmp.c` , struct hook_entry, hooks, add:

```shell
{SF7, proccnt_dmp, "Running process conut"},
```

![1](/Users/billxsy/Desktop/2021FA/VE482/homework/hw2/1.PNG)



In `/usr/src/servers/is/dmp_kernel.c`, add

```c
#include "../pm/mproc.h"

void proccnt_dmp(){
  struct mproc *pc;
  int cnt;
  if (getsysinfo(PM_PROC_NR, SI_PROC_TAB, mproc, sizeof(mproc)) != OK){
    printf("Fail to get Process Count.\n");
		return; 
  }
  int i;
  for(i=0;i<NR_PROCS;i++){ 
    pc=&mproc[i];
    if(pc->mp_pid==0 && i!=PM_PROC_NR) continue; 
	cnt++; 
  }
  printf("Current number of running processes is: %d\n",cnt);
}
```



In `/usr/src/servers/is/proto.h`, add:

```c
void proccnt_dmp(void);
```



Then, 

```shell
cd /usr/src
make build
reboot
```

The result is as follows,![2](/Users/billxsy/Desktop/2021FA/VE482/homework/hw2/2.PNG)
