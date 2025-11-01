Terminal Text Editor (C++/ncurses)

Short, one-sentence elevator pitch of your app.
Example: “A minimal terminal text viewer/editor in C++ using ncurses, built to learn file I/O and terminal UIs.” (Good READMEs start with a clear value statement. ) 
FreeCodeCamp
+2
Everyday DevOps
+2

Features

Open and display a text file line-by-line.

: commands: :q quit, :i enter insert/navigation routine (WIP).

Arrow-key cursor movement (requires keypad(stdscr, TRUE)). 
linux.die.net
+1

Handles CRLF files (strips \r so lines don’t overwrite). 
commandmasters.com
+1

Demo (optional)

Screenshot or short GIF of opening a file and moving the cursor.

Build & Run
Prereqs

Linux/WSL (Kali/Ubuntu) with build tools and ncurses dev headers:

sudo apt update
sudo apt install build-essential libncurses-dev


(Needed to compile & link with -lncurses.) 
Ask Ubuntu

Compile
g++ main.cpp -o textedit -lncurses


(Note: link flags like -lncurses must be present at link time.) 
tilburgsciencehub.com

Run
./textedit

WSL paths (opening Windows files from WSL)

Use /mnt/<drive>/... (e.g., C:\Users\you\file.txt → /mnt/c/Users/you/file.txt). Storing projects under \\wsl$ is faster than mounted Windows dirs. 
Microsoft Learn
+2
Ask Ubuntu
+2

Converting CRLF → LF (if the screen only shows the last line)
dos2unix /mnt/c/path/to/file.txt


(Converts Windows line endings to Unix so \r doesn’t reset the cursor to column 0.) 
commandmasters.com

Usage

Start the program → choose:

1. Open file: type a path (on WSL, use /mnt/...).

2. Create file: (placeholder for future work)

3. Quit

In the viewer:

Arrow keys: move cursor (Y increases downward; X to the right). Enable with keypad(stdscr, TRUE) and use cbreak() to read single keys. 
linux.die.net
+1

: then q: quit

: then i: enter insert/navigation routine (work-in-progress)

Internals (what future-you will forget)

ncurses rendering: move(y,x) updates a virtual cursor; call refresh() to draw on the real screen. Coords are (row, col) with origin (0,0) top-left. 
FreeCodeCamp

Key input: getch() returns key codes; cbreak() for char-wise input, keypad(stdscr, TRUE) for KEY_UP/DOWN/LEFT/RIGHT. 
tldp.org

Streams: after reading to EOF, call file.clear(); file.seekg(0); before reading again (or reopen). 
GitHub

CRLF: if a line ends with \r\n, getline leaves \r. Strip it or dos2unix the file. 
Stack Overflow

WSL performance tip: prefer storing the repo under \\wsl$\<distro>\home\... rather than /mnt/c/... if builds feel slow. 
Microsoft Learn

Project Structure
.
├─ main.cpp        # menu (Setup::setup/state/edit) + Commands::i()
├─ README.md
└─ (future) editor/ buffer/ ui/ …

Roadmap

Insert mode: in-memory edit of lines (insert/delete chars & lines, write back).

Scrolling viewport (firstVisibleLine, re-render on movement).

Status bar (filename, row:col, mode).

Search (/pattern) and jump to line.

Troubleshooting

Linker errors like undefined reference to initscr/printw → missing -lncurses in link step. 
tilburgsciencehub.com

Arrow keys print weird chars → ensure keypad(stdscr, TRUE) and cbreak() are set. 
tldp.org

Only last line shows → convert CRLF with dos2unix or strip \r after getline. 
commandmasters.com

Can’t open C:\... in WSL → use /mnt/c/... mapping. 
Microsoft Learn

License

Choose one (MIT is common for simple learning projects).

Acknowledgements / References

ncurses key handling & keypad docs. 
linux.die.net
+1

Good README guidance & templates. 
Everyday DevOps
+3
FreeCodeCamp
+3
GitHub
+3

CRLF→LF conversions. 
commandmasters.com
+1

WSL paths & performance notes.