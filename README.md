# Rootkit
A simple Linux kernel module


## Features
1. Hides the file "`top_secret`" from the `ls` command
2. Hides an HTTP server (port 80) from the `netstat` command
3. Hides the process with PID `1337` from the `ps` command
4. Hides `ICMP` and `UDP` packets received from `192.168.1.167`
5. Hides from the `lsmod` command while behing able to be removed with `rmmod`

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
