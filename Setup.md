# ENEE150 Glue & Startup Guide

Welcome to ENEE150!
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

---

## 🔹 Setup on macOS

macOS is UNIX-based, so you already have the tools you need.

### 1. Open Terminal
- Press **Cmd + Space** → type `Terminal` → press Enter.

### 2. Connect to Glue
```bash
ssh your_directory_id@glue.umd.edu
```

### 3. Compile and Run on Glue
Inside the terminal:
```bash
gcc hello.c -o hello
./hello
```

---

## 🔹 Transferring Files to Glue

If you wrote your code locally (CLion, VS Code, etc.), you’ll need to transfer your files to Glue to compile and run them.  

1. Open a terminal on your computer.  
2. Use `scp` (secure copy) to upload your entire project directory to Glue:  

   ```bash
   scp -r path/to/directory yourDirectoryID@glue.umd.edu:~/
   ```

   - Replace `path/to/directory` with the location of your project folder.  
   - Replace `yourDirectoryID` with your UMD Directory ID.  

3. You’ll be prompted for your password and Duo push.  

Once this finishes, your project folder will be in your **home directory** on Glue (`~/`).  

---

## 🔹 Compiling and Running Code on Glue

Navigate to your project folder (which you uploaded in the previous step):  

```bash
cd ~/yourProjectFolder
```

Compile your C program (example using `hw1.3.c`):  

```bash
gcc hw1.3.c -o a.out
```

Run your program with an input file (example for problem 3):  

```bash
./a.out < hw1.3.in
```

---

## 🔹 Editing Code Directly on Glue with Emacs

You don’t need an IDE to work on Glue—many developers use terminal editors like **Emacs**.  

### Opening a File in Emacs
```bash
emacs filename.c
```

### Basic Emacs Commands
- **Save**: `Ctrl-x Ctrl-s`  
- **Exit**: `Ctrl-x Ctrl-c`  
- **Cut (kill) line**: `Ctrl-k`  
- **Paste (yank)**: `Ctrl-y`  
- **Undo**: `Ctrl-/`  

*(Tip: If you ever get stuck, press `Ctrl-g` to cancel a command.)*  

### Why Learn Emacs?
- It’s always available on Glue.  
- Great for quick edits without needing to re-upload files.  
- Powerful once you learn the shortcuts.  

---

## 🔹 Recommended Workflow

- **Option A (Most Common):** Write code in your IDE → transfer to Glue with `scp` → compile and run on Glue.  
- **Option B (Advanced/Lightweight):** SSH into Glue → use Emacs to code and compile directly there.  

---

## 🔹 Troubleshooting
- If `scp` fails, make sure you are in the right local directory and that your path is correct.  
- If `ssh` doesn’t work, check your internet connection and Duo app.  
- If compilation fails, check for missing semicolons, headers, or typos.  

---

### Next Steps
This guide combines setup instructions, file transfer, compilation, and editing methods.  
Good luck, and feel free to ask if you run into issues!