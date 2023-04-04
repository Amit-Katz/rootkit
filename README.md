# Rootkit
A simple Linux kernel module


## Features
1. Hides the file "`top_secret`" from the `ls` command

## Usage

Compiling and installing:
```bash
$ make
$ sudo insmod rootkit.ko
```

Uninstall:
```bash
$ sudo rmmod rootkit
```
