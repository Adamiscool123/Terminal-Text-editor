# Terminal Text Editor (C++ / ncurses)

A minimal terminal text viewer/editor written in **C++** with **ncurses**. It opens a file, renders it line-by-line, and lets you move a cursor and run simple `:` commands (`:q`, `:i`). This is a learning project focused on terminal UI, file I/O, and keystroke handling.

---

## Features
- Open and display a text file line-by-line.
- Arrow-key cursor movement on the rendered text.
- `:` commands:
  - `:q` — quit viewer
  - `:i` — enter insert/navigation routine (work-in-progress)
- Handles Windows CRLF files by stripping `\r` so lines don’t overwrite.

---

## How It Works (Quick Overview)
- **ncurses rendering**: You update a *virtual* screen; `refresh()` pushes it to the terminal. Cursor moves with `move(y, x)` (origin top-left `(0,0)`, `y` = row, `x` = column).
- **Input**: `getch()` returns key codes. Use `cbreak()` to get keys immediately and `keypad(stdscr, TRUE)` so arrow keys arrive as `KEY_UP/DOWN/LEFT/RIGHT`.
- **Streams**: After reading to EOF, call `clear()` **then** `seekg(0)` (or reopen) before reading again.
- **CRLF**: If the file came from Windows, each line may end with `\r\n`. `getline` strips `\n` but leaves `\r`. The code trims trailing `\r` to prevent cursor resets.

---

## Build

Install build tools and ncurses headers (Debian/Ubuntu/Kali/WSL):

```bash
sudo apt update
sudo apt install build-essential libncurses-dev
```

Compile:

```bash
g++ main.cpp -o textedit -lncurses
```

> Note: The `-lncurses` link flag is required at link time.

---

## Run

```bash
./textedit
```

### Opening Windows files from WSL
Use the `/mnt/<drive>/...` mapping, e.g. `C:\Users\you\file.txt` → `/mnt/c/Users/you/file.txt`.

### “Only last line shows”?
That’s typically CRLF input. Convert once with:
```bash
dos2unix /path/to/file.txt
```
(Or rely on the built-in `\r` stripping.)

---

## Usage

1. Start the program → Menu:
   - **1. Open file** — enter a path (on WSL, use `/mnt/...`).
   - **2. Create file** — *placeholder for future work*.
   - **3. Quit**

2. In the viewer:
   - **Arrow keys** — move the cursor (rows increase downward).
   - **`:` then `q`** — quit.
   - **`:` then `i`** — enter insert/navigation routine (WIP).

---

## Project Structure (Current)

```
.
├─ main.cpp        # menu (Setup::setup/state/edit) + Commands::i()
└─ README.md
```

---

## Internals You’ll Want to Remember
- `move(y, x)` is **(row, col)**; you must call `refresh()` to see it.
- `cbreak()` + `keypad(stdscr, TRUE)` are needed so `getch()` returns arrow keys as `KEY_*`.
- When reusing a `std::fstream` after EOF: `file.clear(); file.seekg(0);`.
- Keep cursor bounds within the window size (`LINES`, `COLS`) and within the current line length for horizontal moves.

---

## Roadmap
- Real insert mode (edit `lines` in memory; write back to disk).
- Scrolling viewport (`firstVisibleLine`, re-render region).
- Status bar (filename, row:col, mode).
- Search (`/pattern`), jump to line.

---

## Troubleshooting
- **Undefined references to `initscr/printw`** — link with `-lncurses`.
- **Arrow keys print weird characters** — ensure `cbreak()` and `keypad(stdscr, TRUE)` are set.
- **Can’t open `C:\...` from WSL** — use `/mnt/c/...` mapping.
- **Screen doesn’t update** — call `refresh()` after drawing or moving the cursor.

---

## License
MIT (or your choice).
