// Declares the Setup class, responsible for menu handling and editor entry.
#ifndef _SETUP_H_
#define _SETUP_H_

#include <iostream>
#include <string>

class Setup{
public:
    int choice = 0;              // menu selection (1=open, 2=create, 3=quit)

    std::string file_path;       // path to the currently selected file

    void setup();                // draw main menu and read choice
    void state();                // branch based on choice
    void modes();                 // view file and await :commands
    void openfile();
};

#endif //_SETUP_H_
