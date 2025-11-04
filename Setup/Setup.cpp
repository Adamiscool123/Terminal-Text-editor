// Implements menu flow and file-opening logic for the terminal editor.
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
#include "Setup.h"
#include "Commands/Commands.h"

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

void Setup::openfile(){
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
}

void Setup::state()
{
    if (choice == 1)
    {
        openfile();

        modes();                    // proceed to viewer/command loop
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

void Setup::modes()
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

                l.insert(second_file);                    // enter insert-mode handler
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
