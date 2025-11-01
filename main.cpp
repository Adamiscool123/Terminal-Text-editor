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

class Setup
{
public:
    int choice = 0;              // menu selection (1=open, 2=create, 3=quit)

    std::string file_path;       // path to the currently selected file

    void setup();                // draw main menu and read choice
    void state();                // branch based on choice
    void edit();                 // view file and await :commands
};

class Commands : public Setup
{
public:
    void i(std::fstream&);       // “insert-mode” entry point; receives a stream
};

void Commands::i(std::fstream& file)
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

    int y = 0;
    int x = 0;
    int maxY, maxX;
    getmaxyx(stdscr, maxY, maxX);

    move(y, x);            // position cursor at (row=y, col=x)
    refresh();  

    while (true) {
        int key = getch();
        if (key == KEY_UP) {
            if (y > 0) {
                y--;
                if (x > (int)lines[y].size() - 1) {
                    x = lines[y].size() ? lines[y].size() - 1 : 0;
                }
            }
        }
        else if (key == KEY_DOWN) {
            if (y + 1 < (int)lines.size() && y + 1 < maxY) {
                y++;
                if (x > (int)lines[y].size() - 1) {
                    x = lines[y].size() ? lines[y].size() - 1 : 0;
                }
            }
        }
        else if (key == KEY_LEFT) {
            if (x > 0) {
                x--;
            }
        }
        else if (key == KEY_RIGHT) {
            if (x + 1 < (int)lines[y].size() && x + 1 < maxX) {
                x++;
            }
        }
        else{
            char buf[300];

            while(true){
                echo();

                getstr(buf);

                std::string userInput = buf;
                
                mvprintw(y+1, x, "%s", userInput.c_str());
                
                refresh();
            }
        }

        move(y, x);
        refresh();
    }
}

void Setup::setup()
{
    clear();                       // clear screen for menu
    echo();                        // show typed characters for menu input

    // simple menu
    printw("1. Open file\n");
    printw("2. Create file\n");
    printw("3. Quit\n");
    printw("\nChoice: ");

    refresh();                     // flush menu to screen
    scanw("%d", &choice);          // read numeric choice (blocks)

    state();                       // handle the selected option
}

void Setup::state()
{
    if (choice == 1)
    {
        clear();

        int checker = 0;           // loop until a valid file path is provided
        char buff[300];            // buffer for path via scanw("%s", ...)

        printw("File Path: ");
        refresh();

        scanw("%s", buff);         // read path (stops at whitespace)
        file_path = buff;

        std::ifstream file;        // used only to test open success

        while (checker == 0)
        {
            file.open(file_path, std::ios::in);  // try opening for read

            if (!file)
            {
                // open failed: prompt again
                clear();
                printw("Could not open file\n\n");
                printw("Please try again: ");
                refresh();

                scanw("%s", buff);
                file_path = buff;

                file.clear();      // reset flags before next open attempt
            }
            else
            {
                checker = 1;       // success
                file.clear();      // clear flags (not strictly needed here)
            }
        }

        file.close();              // close test stream

        edit();                    // proceed to viewer/command loop
    }
    else if (choice == 2)
    {
        // placeholder for “create file” branch
    }
    else if (choice == 3)
    {
        // quit selected; main loop will exit when choice == 3
    }
    else
    {
        // invalid menu input case (not handled explicitly)
    }
}

void Setup::edit()
{
    clear();

    Commands l;                    // create a Commands object to call i()

    std::fstream second_file;      // shared read/write stream

    // open file for read/write with append flag (you’re using this mode now)
    // note: app forces writes to end; you also read from it below
    second_file.open(file_path, std::ios::in | std::ios::out | std::ios::app);

    std::string content;           // line buffer for initial display

    second_file.seekg(0, std::ios::beg);  // rewind to start for reading

    // initial file dump to screen
    while (std::getline(second_file, content))
    {
        if (!content.empty() && content.back() == '\r') content.pop_back(); // strip CR
        printw("%s\n", content.c_str());
    }

    noecho();                      // stop echoing typed keys for command mode

    char letter;

    // command loop: waits for ":" then a command char (q/i/v)
    while (true){
        int ch = getch();

        if (ch == ':'){
            int ch_2 = getch();    // read the command character

            if(ch_2 == 'q'){
                second_file.close();  // close file and exit to menu
                break;
            }
            else if(ch_2 == 'i'){
                // preparing the stream for reuse in Commands::i
                second_file.clear();                 // clear EOF/fail flags
                second_file.seekg(0, std::ios::beg); // rewind to start

                l.i(second_file);                    // enter insert-mode handler
            }
            else if (ch == ':')
            {
                // nested ':' branch placeholder (currently unused)
                int ch_2 = getch();

                if(ch_2 == 'v'){
                    // placeholder for a 'view' command
                }
            }
        }
    }
    echo();                        // restore echo before returning
}

int main()
{
    Setup s;

    initscr();                     // initialize ncurses
    cbreak();                      // disable line buffering (getch returns immediately)
    noecho();                      // don’t echo typed characters by default
    keypad(stdscr, TRUE);          // enable function/arrow keys as KEY_*

    while (s.choice != 3)          // loop until user chooses Quit
    {
        s.setup();                 // draw menu and handle one action
    }

    endwin();                      // restore terminal to normal mode

    return 0;                      // normal exit
}
