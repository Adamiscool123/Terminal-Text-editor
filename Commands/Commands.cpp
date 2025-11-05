// Implements cursor movement and insert-mode behavior for opened files.
#ifdef _WIN32
  #include <curses.h>    // Windows / PDCurses
#else
  #include <ncurses.h>   // Linux / ncurses
#endif
#include <iostream>      // standard I/O streams (cout/cin)
#include <fstream>       // file streams (ifstream/ofstream/fstream)
#include <iomanip>       // I/O manipulators (not used yet)
#include <filesystem>    // C++17 filesystem utilities (not used yet)
#include <stdio.h>       // C stdio (not used directly)
#include <string>        // std::string
#include <vector>        // std::vector
#include <cstdlib>       // general utilities
#include <sstream>       // string streams (not used yet)

#include "Commands.h"

void Commands::keyup(std::vector<std::string> lines, int &y, int &x){
    if (y > 0) {
        y--;
        
        //Check if x is bigger than lines[y] size
        if (x > (int)lines[y].size() - 1) {
            //check if lines[y] size is bigger than 0 (has things in the line)
            if (lines[y].size() > 0) {
                //if so then set x = lines[y] size
                x = (int)lines[y].size() - 1;
            //if there is nothing in row y then set x = 0 for row y
            } else {
                x = 0;
            }
        }
    }
}

void Commands::keydown(std::vector<std::string> lines, int &y, int &x, int maxY){
    if (y + 1 < (int)lines.size() && y + 1 < maxY) {
        y++;
        
        //Check if x is bigger than lines[y] size
        if (x > (int)lines[y].size() - 1) {
            //check if lines[y] size is bigger than 0 (has things in the line)
            if (lines[y].size() > 0) {
                //if so then set x = lines[y] size
                x = (int)lines[y].size() - 1;
            //if there is nothing in row y then set x = 0 for row y
            } else {
                x = 0;
            }
        }
    }
}

void Commands::writing(std::fstream& file, int position, char* buf){
    echo();
    
    getnstr(buf, sizeof(buf));

    std::string userInput = buf;

    file.seekp(position);

    file << userInput;
    
    refresh();
}

void Commands::insert(std::fstream& file)
{
    clear();                     // ncurses: clear the screen buffer

    std::vector<std::string> lines;  // holds all lines from the file
    std::string line;                // scratch line buffer

    int count = 0;               // number of lines printed

    // rewind the passed-in stream to beginning (caller should also clear flags)
    // note: if EOF/fail flags are set, seekg alone won’t work; caller clears()
    // (you already do that in Setup::edit before calling i)
    file.seekg(0, std::ios::beg);

    // read and render file content line-by-line
    while (std::getline(file, line))
    {
        // strip trailing '\r' if the file has CRLF endings (Windows)
        if (!line.empty() && line.back() == '\r') line.pop_back();

        lines.push_back(line);                     // store the line

        printw("%s", lines.at(count).c_str());    // print the line
        printw("\n");                             // newline after it
        refresh();                                 // flush to terminal

        count++;                                   // increment printed count
    }

    int y = 0; //y coordinate
    int x = 0; //x coordinate
    int maxY, maxX; //max_y and max_x values
    getmaxyx(stdscr, maxY, maxX); //set dimensions of window

    int position = 0;
    
    for (int i = 0; i < y; ++i) {
        position += lines[i].size() + 1; // +1 for newline character
    }
    position+=x;

    char *buf = new char[300]; //user input 

    move(y, x); // position cursor at (row=y, col=x)
    refresh();  

    while (true) {
        //capture input
        int key = getch();

        //Capture arrow key up
        if (key == KEY_UP) {
            keyup(lines, y, x);
        }
        //Capture arrow key down
        else if (key == KEY_DOWN) {
            keydown(lines, y, x, maxY);
        }
        //Capture arrow key left
        else if (key == KEY_LEFT) {
            if (x > 0) {
                x--;
            }
        }
        //Capture arrow key right
        else if (key == KEY_RIGHT) {
            x++;
        }
        //Capture arrow key space
        else if(key == ' '){
            x++;

            move(y, x);

            refresh();
        }
        //Capture arrow key backspace
        else if(key == KEY_BACKSPACE){
            x--;

            mvprintw(y, x, " ");

            refresh();
        }
        //Capture arrow key enter
        else if(key == KEY_ENTER){
            y++;

            mvprintw(y, x, "\n");

            refresh();
        }
        else{
            writing(file, position, buf);
        }

        move(y, x);
        refresh();
    }
}
