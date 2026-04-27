# Linux System Programming

> Collection of Linux system programming programs implemented using low-level system calls and concepts.

---

## 📌 Overview

This repository contains implementations of core Linux system programming concepts including file operations, networking using sockets, and advanced topics like framebuffer programming.

---

## 📂 Repository Structure

```
file-operations/
    my_cat.c
    my_cp.c
    my_head.c
    my_tail.c
    wc.c

networking/
    client.c
    server.c

advanced/
    framebuffer/
        bmp_display.c
```

---

## ⚙️ Concepts Covered

### 🔹 File Operations

* open(), read(), write(), close()
* Implementations of:

  * cat
  * cp
  * head
  * tail
  * wc

---

### 🔹 Networking (Sockets)

* TCP client-server communication
* socket(), bind(), listen(), accept(), connect()
* Multithreading using pthread

---

### 🔹 Advanced (Framebuffer)

* Direct screen access using `/dev/fb0`
* Memory mapping using mmap()
* Device control using ioctl()
* Displaying BMP images on screen

---

## 🛠️ Build Instructions

```bash
make
```

### Clean binaries:

```bash
make clean
```

---

## 🚀 How to Run

### File Operations

```bash
./my_cat file.txt
./my_cp src.txt dest.txt
./my_head file.txt
./my_tail file.txt
./wc file.txt
```

---

### Networking

```bash
./server
./client
```

---

### BMP Display (Advanced)

```bash
./bmp_display image.bmp
```

> ⚠️ Requires Linux system with framebuffer support (`/dev/fb0`)

---

## 🎯 Key Learnings

* Low-level system call usage
* File handling in Linux
* Socket programming and IPC
* Multithreading using pthreads
* Memory mapping and device interaction

---

## 📌 Note

This repository is created for learning and demonstrating Linux system programming concepts with clean and structured implementations.

---
