#include <iostream>
#include <fstream>
#include <iomanip>
#include <filesystem>
#include <stdio.h>
#include <string>
#include <vector>
#include <cstdlib>
#include <ncurses.h>
#include <sstream>

class Setup
{
public:
    int choice = 0;

    std::string file_path;

    void setup();

    void state();

    void edit();
};

class Commands : public Setup
{
public:
    void i(std::fstream&);
};

void Commands::i(std::fstream& file)
{
    clear();

    std::vector<std::string> lines;

    std::string line;

    int count = 0;

    int y = 0;

    int x = 0;

    while (std::getline(file, line))
    {
        if (!line.empty() && line.back() == '\r') line.pop_back();

        lines.push_back(line);

        printw("%s", lines.at(count).c_str());

        printw("\n");

        refresh();

        count++;
    }

    move(y, x);

    refresh();

    int numlines = count;

    int numrow = lines.size();

    while (true)
    {
        int keys = getch();

        if (keys == KEY_UP)
        {
            if (y == 0)
            {
            }
            else
            {
                y--;
            }
        }
        else if (keys == KEY_DOWN)
        {
            if (y == numrow)
            {
            }
            else
            {
                y++;
            }
        }
        else if (keys == KEY_RIGHT)
        {
            if (x == numlines)
            {
            }
            else
            {
                x++;
            }
        }
        else if (keys == KEY_LEFT)
        {
            if (x == 0)
            {
            }
            else
            {
                x--;
            }
        }

        move(y, x);

        refresh();
    }
}

void Setup::setup()
{
    clear();
    echo();

    printw("1. Open file\n");
    printw("2. Create file\n");
    printw("3. Quit\n");
    printw("\nChoice: ");

    refresh();
    scanw("%d", &choice);

    state();
}

void Setup::state()
{
    if (choice == 1)
    {
        clear();

        int checker = 0;

        char buff[300];

        printw("File Path: ");
        refresh();

        scanw("%s", buff);
        file_path = buff;

        std::ifstream file;

        while (checker == 0)
        {
            file.open(file_path, std::ios::in);

            if (!file)
            {
                clear();
                printw("Could not open file\n\n");

                printw("Please try again: ");

                refresh();

                scanw("%s", buff);

                file_path = buff;

                file.clear();
            }
            else
            {
                checker = 1;

                file.clear();
            }
        }

        file.close();

        edit();
    }
    else if (choice == 2)
    {
    }
    else if (choice == 3)
    {
    }
    else
    {
    }
}

void Setup::edit()
{
    clear();

    Commands l;

    std::fstream second_file;

    second_file.open(file_path, std::ios::in | std::ios::out | std::ios::app);

    std::string content;

    second_file.seekg(0, std::ios::beg);

    while (std::getline(second_file, content))
    {
        if (!content.empty() && content.back() == '\r') content.pop_back();
        printw("%s\n", content.c_str());
    }

    noecho();

    while (true){

        int ch = getch();

        if (ch == ':'){
            int ch_2 = getch();
            
            if(ch_2 == 'q'){
                second_file.close();

                break;
            }
            else if(ch_2 == 'i'){
                second_file.clear();                 // clear eof/fail after earlier reads
                
                second_file.seekg(0, std::ios::beg);

                l.i(second_file);
            }
            else if (ch == ':')
            {
                int ch_2 = getch();

                if(ch_2 == 'v'){
                
                }

            }
        }
    }
    echo();
}

int main()
{
    Setup s;

    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    while (s.choice != 3)
    {
        s.setup();
    }

    endwin();

    return 0;
}