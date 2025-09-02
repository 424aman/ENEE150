# ENEE150 Startup Guide

Welcome to ENEE150! 🎉  
This guide will help you get set up with the **Glue UNIX environment** on both macOS and Windows.  
All assignments will be compiled and graded on Glue, so it’s critical that your code runs there.

---

## 🔹 What is Glue?

Glue is UMD’s campus-wide UNIX computing system. It’s a distributed network of servers with a shared file system.  
When you log in to `glue.umd.edu`, you are accessing your personal home directory — the same no matter which Glue machine you use.

- **Why it matters:** Assignments are graded on Glue.  
- **Tip:** Even if you code on your own laptop, always test on Glue before submitting.

---

## 🔹 Accounts

- You’ll log in with your **UMD Directory ID** and password.
- Hostname: `glue.umd.edu`.

---

## 🔹 Setup on Windows (Recommended: MobaXterm)

For Windows, the easiest option is **MobaXterm** — it combines SSH and file transfer in one program.

### 1. Install MobaXterm
- Download the free Home Edition: [https://mobaxterm.mobatek.net/download.html](https://mobaxterm.mobatek.net/download.html)
- Install and launch the program.

### 2. Create a New Session
1. Open MobaXterm.  
2. Click **Session → SSH**.  
3. In **Remote host**, enter: glue.umd.edu
4. Check **Specify username** and enter your **UMD Directory ID**.  
5. (Optional) Enable **X11 forwarding** if you ever need GUI programs (not common in ENEE150).  
6. Save the session for quick future access.

### 3. Connect
- Double-click your saved session.  
- **Left pane:** file browser (drag and drop files between your PC and Glue).  
- **Right pane:** terminal session on Glue (type UNIX commands here).  

### 4. Compile and Run on Glue
Inside the terminal:
```bash
gcc hello.c -o hello
./hello
```
## 🔹 Setup on macOS

macOS is UNIX-based, so you already have the tools you need.

### 1. Open Terminal
- Press **Cmd + Space** → type `Terminal` → press Enter.

### 2. Connect to Glue

ssh your_directory_id@glue.umd.edu

### 3. Compile and Run on Glue
Inside the terminal:
```bash
gcc hello.c -o hello
./hello
```