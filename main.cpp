// Entry point for the ncurses-based terminal editor; wires menu flow to Setup.
#include <iostream>      // standard I/O streams (cout/cin)
#include <fstream>       // file streams (ifstream/ofstream/fstream)
#include <iomanip>       // I/O manipulators (not used yet)
#include <filesystem>    // C++17 filesystem utilities (not used yet)
#include <stdio.h>       // C stdio (not used directly)
#include <string>        // std::string
#include <vector>        // std::vector
#include <cstdlib>       // general utilities
#include <ncurses.h>     // ncurses screen/keyboard handling
#include <sstream>       // string streams (not used yet)
#include "Setup/Setup.h"
#include "Commands/Commands.h"

int main()
{
    Setup s;

    initscr();                     // initialize ncurses
    cbreak();                      // disable line buffering (getch returns immediately)
    noecho();                      // don’t echo typed characters by default
    keypad(stdscr, TRUE);          // enable function/arrow keys as KEY_*
    ESCDELAY = 25;

    while (s.choice != 3)          // loop until user chooses Quit
    {
        s.setup();                 // draw menu and handle one action
    }

    endwin();                      // restore terminal to normal mode

    return 0;                      // normal exi
}
