<div style="width:60%;height:200px;text-align:center;border:14px solid #808080;border-top:none;border-left:none;border-bottom:none;display:inline-block">
    <div style="border:4px solid #808080;border-radius:8px;width:95%;height:100%;background-color: rgb(209, 209, 209);">
        <div style="width:100%;height:30%;text-align:center;line-height:60px;font-size:26px;font-family:'Lucida Sans', 'Lucida Sans Regular', 'Lucida Grande', 'Lucida Sans Unicode', Geneva, Verdana, sans-serif;">VE482 Lab Report</div>
        <div style="width:100%;height:18%;text-align:center;line-height:26px;font-size:20px;font-familny:'Lucida Sans', 'Lucida Sans Regular', 'Lucida Grande', 'Lucida Sans Unicode', Geneva, Verdana, sans-serif;"><b>Lab 3</b> - Fall 2020</div>
        <div style="width:100%;height:57%;text-align:center;font-size:16px;line-height:22px;font-family: 'Courier New', Courier, monospace;font-weight:300;"><br><b>Name: Shengyuan Xu<br>ID: 518370910200<br>Email: xushengyuan@sjtu.edu.cn<br></b></div>
    </div>
</div>
<div style="width:35%;height:200px;display:inline-block;float:right">
    <div style="width:100%;height:25%;text-align:center;line-height:55px;font-size:20px;font-family:'Lucida Sans', 'Lucida Sans Regular', 'Lucida Grande', 'Lucida Sans Unicode', Geneva, Verdana, sans-serif;"><b>Table of Contents</b></div>
    <div style="width:100%;height:75%;text-align:left;margin-left:2px;line-height:30px;font-size:13px;font-family:Verdana, Geneva, Tahoma, sans-serif;font-weight:300;">• Be ready to use git in the course<br>• Work with source code<br>• Prepare for project 1<br>• Regular expressions</div>
</div>

[TOC]
## 1 Project 1: presentations



## 2 Working with source code

### 2.1 The `rsync` command

```shell
# In Minix 3 install the rsync software
pkgin install rsync

# Install rsync on you Linux system
apt-get install rsync

# Read `rsync` manpage
man rsync

# Create an exact copy of the directory /usr/src into the directory /usr/src_orig
rsync -av /usr/src /usr/src_orig

# If you have already altered Minix3 source code for homework2 undo your changes from /usr/src_orig
rsync -av /usr/src_orig /usr/src

# Create an exact copy of the Minix 3 directory /usr/src into your Linux system, using rsync and ssh (note that the ssh server must be activated under Linux)
rsync -av -e 'ssh -p 2222' root@192.168.164.128:/usr/src ~/
```



### 2.2 The`diff` and `patch` commands

```shell
# Read the manpages of diff and patch
man diff
man patch

# Edit a file of your choice in /usr/src
# Using the diff command, create a patch corresponding to the above changes
echo "111" >> src/test
diff -u src_orig/test src/test > patch

# Retrieve your patch on your Linux system
rsync -av -e 'ssh -p 2222' root@192.168.164.128:/root/patch /usr

# Apply your patch to the copy of /usr/src_orig on your Linux system
cd /usr
patch -p2 < patch

# Revert the patch
patch -p2 -R < patch

```



### 2.3 Remarks



### 2.4 Basic git usage

![image-20211007223752312](/Users/billxsy/Library/Application Support/typora-user-images/image-20211007223752312.png)

![image-20211007223757171](/Users/billxsy/Library/Application Support/typora-user-images/image-20211007223757171.png)





## 3 Scripting and regular expressions

### 3.1  Shanghai air quality

```shell
curl -s 'http://aqicn.org/?city=Shanghai&widgetscript&size=large&id=52b39d71decf07.20261781' | sed -rn 's/.*hdrpm25[^>]*>([0-9]+).*hdrt[^>]*>([0-9]+).*/AQ: \1 Temp: \2 ºC/p'
```

![image-20211007223959642](/Users/billxsy/Library/Application Support/typora-user-images/image-20211007223959642.png)



### 3.2 IP address

```shell
ifconfig eth0 | awk -e '{for(j=1;j<=NF;j++){if($j=="inet"){print $(j+1)}}}'
```

![image-20211007233039696](/Users/billxsy/Library/Application Support/typora-user-images/image-20211007233039696.png)

