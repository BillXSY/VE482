# VE482 — Introduction to Operating Systems Homework 6



## Ex. 1 — Simple questions

### 1. 

​	(i) first fit: 20 KB, 10 KB, 18KB

​	(ii) best fit: 12KB, 10 KB, 9KB

​	(iii) quick fit: 12KB, 10 KB, 9KB

### 2.

$$\frac{10k+n}{k} = 10 + \frac{n}{k}$$

### 3.

```c++
01101110 // Page 0
01001001 // Page 1
00110111 // Page 2
10001011 // page 3
```



## Ex. 2 — Page tables

### Inverted Page Tables<sup>1</sup>



![image-20211124201940021](/Users/billxsy/Library/Application Support/typora-user-images/image-20211124201940021.png)

​	An **inverted page table** is one of the techniques to structure a *page table*, where the table is indexed by the actual *frame number* in the physical memory. The entry in the table corresponds to the logical page number § and the process-id (pid) of the process that owns that page. The tuple (pid,p) is extracted whenever a memory reference happens and is forwarded to the memory subsystem that contains the inverted page table.

​	The memory subsystem searches through the inverted page table for a match. If a match is found at an entry, such as i*i*, then the corresponding physical address consists of i*i* and the offset (d). If no match is found, this means an address was accessed illegally. The 64-bit UltraSPARC and PowerPC are common examples of systems that use inverted page tables.

#### Why use the inverted page table?

​	As you can observe, an inverted page table has exactly one entry for each physical frame in the real memory, and therefore only one inverted page table exists in an operating system. This is in contrast to the standard *hierarchical* page tables, which are indexed by logical address pages, and each process has its own separate page table.

​	Since a process references pages using logical addresses, hierarchical page tables seem to be the natural way to structure a page table. The translation of addresses is therefore a constant order of operation.

​	However, the drawback of this technique is that each page table corresponding to a process may contain millions of entries just to track how physical memory is being used. This consumes large amounts of physical memory, which may be undesirable. In such a scenario, the *inverted* page table solves this problem by only having entries corresponding to actual physical frames, and there is only a page table in the whole system.



### Multilevel Page Tables<sup>2</sup>

![img](https://media.geeksforgeeks.org/wp-content/uploads/20190608174704/multilevel.png)

**Multilevel Paging** is a paging scheme which consist of two or more levels of page tables in a hierarchical manner. It is also known as hierarchical paging. The entries of the level 1 page table are pointers to a level 2 page table and entries of the level 2 page tables are pointers to a level 3 page table and so on. The entries of the last level page table are stores actual frame information. Level 1 contain single page table and address of that table is stored in PTBR (Page Table Base Register)

In multilevel paging whatever may be levels of paging all the page tables will be stored in main memory.So it requires more than one memory access to get the physical address of page frame. One access for each level needed. Each page table entry **except** the last level page table entry contains base address of the next level page table. 



## Ex. 3 — Research

### Security Bug<sup>3</sup>

​	A security bug or security defect is a software bug that can be exploited to gain unauthorized access or privileges on a computer system. Security bugs introduce security vulnerabilities by compromising one or more of:

- Authentication of users and other entities
- Authorization of access rights and privileges
- Data confidentiality
- Data integrity

​	Security bugs do not need be identified nor exploited to be qualified as such and are assumed to be much more common than known vulnerabilities in almost any system.

#### gets

The stdio `gets()` function does not check for buffer length and always results in a vulnerability.

```c
#include <stdio.h>
int main () {
    char username[8];
    int allow = 0;
    printf ("Enter your username, please: ");
    gets(username); // user inputs "malicious"
    if (grantAccess(username)) {
        allow = 1;
    }
    if (allow != 0) { // has been overwritten by the overflow of the username.
        privilegedAction();
    }
    return 0;
}
```

#### Mitigation

Prefer using fgets (and dynamically allocated memory!): 

```c
#include <stdio.h>
#include <stdlib.h>
#define LENGTH 8
int main () {
    char* username, *nlptr;
    int allow = 0;
 
    username = malloc(LENGTH * sizeof(*username));
    if (!username)
        return EXIT_FAILURE;
    printf ("Enter your username, please: ");
    fgets(username,LENGTH, stdin);
    // fgets stops after LENGTH-1 characters or at a newline character, which ever comes first.
    // but it considers \n a valid character, so you might want to remove it:
    nlptr = strchr(username, '\n');
    if (nlptr) *nlptr = '\0';
 
    if (grantAccess(username)) {
        allow = 1;
    }
    if (allow != 0) {
        priviledgedAction();
    }
 
    free(username);
 
    return 0;
}
```



### Meltdown and Spectre<sup>4</sup>

Meltdown and Spectre exploit critical vulnerabilities in modern processors. These hardware vulnerabilities allow programs to steal data which is currently processed on the computer. While programs are typically not permitted to read data from other programs, a malicious program can exploit Meltdown and Spectre to get hold of secrets stored in the memory of other running programs. This might include your passwords stored in a password manager or browser, your personal photos, emails, instant messages and even business-critical documents. 

Meltdown and Spectre work on personal computers, mobile devices, and in the cloud. Depending on the cloud provider's infrastructure, it might be possible to steal data from other customers.

#### Meltdown

Meltdown breaks the most fundamental isolation between user applications and the operating system. This attack allows a program to access the memory, and thus also the secrets, of other programs and the operating system.

If your computer has a vulnerable processor and runs an unpatched operating system, it is not safe to work with sensitive information without the chance of leaking the information. This applies both to personal computers as well as cloud infrastructure. Luckily, there are software patches against Meltdown.

#### Spectre

Spectre breaks the isolation between different applications. It allows an attacker to trick error-free programs, which follow best practices, into leaking their secrets. In fact, the safety checks of said best practices actually increase the attack surface and may make applications more susceptible to Spectre

Spectre is harder to exploit than Meltdown, but it is also harder to mitigate. However, it is possible to prevent specific known exploits based on Spectre through software patches.





### Dirty COW bug<sup>5</sup>

[![DirtyCow.svg](https://upload.wikimedia.org/wikipedia/commons/thumb/1/1b/DirtyCow.svg/180px-DirtyCow.svg.png)](https://en.wikipedia.org/wiki/File:DirtyCow.svg)

​	Dirty COW (Dirty copy-on-write) is a computer security vulnerability for the Linux kernel that affected all Linux-based operating systems, including Android devices, that used older versions of the Linux kernel created before 2018. It is a local privilege escalation bug that exploits a race condition in the implementation of the copy-on-write mechanism in the kernel's memory-management subsystem. Computers and devices that still use the older kernels remain vulnerable.

​	The vulnerability was discovered by Phil Oester.Because of the race condition, with the right timing, a local attacker can exploit the copy-on-write mechanism to turn a read-only mapping of a file into a writable mapping. Although it is a local privilege escalation, remote attackers can use it in conjunction with other exploits that allow remote execution of non-privileged code to achieve remote root access on a computer. The attack itself does not leave traces in the system log.

​	The vulnerability has the Common Vulnerabilities and Exposures designation CVE-2016-5195.[3] Dirty Cow was one of the first security issues transparently fixed in Ubuntu by the Canonical Live Patch service.

​	It has been demonstrated that the vulnerability can be utilized to root any Android device up to Android version 7.



## Ex. 4 — Linux

```c
#include <stdio.h>
#include <stdlib.h>

typedef struct _Big {
	int *d1; // 4GB
	int *d2; // 4GB
	int *d3; // 4GB
	int *d4; // 4GB
	int *d5; // 4GB
	int *d6; // 4GB
	int *d7; // 4GB
	int *d8; // 4GB
} Big;

void initBit(Big *data);

int main() {
	Big data;
	initBit(&data);
	free(data.d1);
	free(data.d2);
	free(data.d3);
	free(data.d4);
	free(data.d5);
	free(data.d6);
	free(data.d7);
	free(data.d8);
	return 0;
}

void initBit(Big *data) {
	data->d1 = (int *) malloc(sizeof(int) * 1073741824);
	printf("d1 init ok\n");
	data->d2 = (int *) malloc(sizeof(int) * 1073741824);
	printf("d2 init ok\n");
	data->d3 = (int *) malloc(sizeof(int) * 1073741824);
	printf("d3 init ok\n");
	data->d4 = (int *) malloc(sizeof(int) * 1073741824);
	printf("d4 init ok\n");
	data->d5 = (int *) malloc(sizeof(int) * 1073741824);
	printf("d5 init ok\n");
	data->d6 = (int *) malloc(sizeof(int) * 1073741824);
	printf("d6 init ok\n");
	data->d7 = (int *) malloc(sizeof(int) * 1073741824);
	printf("d7 init ok\n");
	data->d8 = (int *) malloc(sizeof(int) * 1073741824);
	printf("d8 init ok\n");

	for (int i = 0; i < 1073741824; i++) (data->d1)[i] = i;
	printf("d1 calc ok\n");
	for (int i = 0; i < 1073741824; i++) (data->d2)[i] = i;
	printf("d2 calc ok\n");
	for (int i = 0; i < 1073741824; i++) (data->d3)[i] = i;
	printf("d3 calc ok\n");
	for (int i = 0; i < 1073741824; i++) (data->d4)[i] = i;
	printf("d4 calc ok\n");
	for (int i = 0; i < 1073741824; i++) (data->d5)[i] = i;
	printf("d5 calc ok\n");
	for (int i = 0; i < 1073741824; i++) (data->d6)[i] = i;
	printf("d6 calc ok\n");
	for (int i = 0; i < 1073741824; i++) (data->d7)[i] = i;
	printf("d7 calc ok\n");
	for (int i = 0; i < 1073741824; i++) (data->d8)[i] = i;
	printf("d8 calc ok\n");
}

```

### Test Result

![image-20211124233126477](/Users/billxsy/Library/Application Support/typora-user-images/image-20211124233126477.png)

![image-20211124233154266](/Users/billxsy/Library/Application Support/typora-user-images/image-20211124233154266.png)

![image-20211124233105693](/Users/billxsy/Library/Application Support/typora-user-images/image-20211124233105693.png)

My computer has physical memory of 16 GB, so theoretically d1~d4 are enough to trigger thrashing. But due to the auto usage of virtual memory, it can handle more than 32 GB. The peaks in the figures are severals tests I made to trigger thrashing.



## Reference

[1] https://www.educative.io/edpresso/what-is-an-inverted-page-table-in-operating-systems

[2] https://www.geeksforgeeks.org/multilevel-paging-in-operating-system/

[3] https://security.web.cern.ch/recommendations/en/codetools/c.shtml

[4] https://meltdownattack.com

[5] https://arstechnica.com/security/2016/10/most-serious-linux-privilege-escalation-bug-ever-is-under-active-exploit/
