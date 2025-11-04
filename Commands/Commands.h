// Declares text-editing commands invoked from Setup::edit (e.g., insert mode).
#ifndef _COMMANDS_H_
#define _COMMANDS_H_

#include <iostream>
#include <fstream>

class Commands{
public:
    void insert(std::fstream&);       // “insert-mode” entry point; receives a stream
    void keyup(std::vector<std::string>, int &y, int &x);
    void keydown(std::vector<std::string> lines, int &y, int &x, int maxY);
    void writing(std::fstream& file, int position, char* buf);
};

#endif //_COMMANDS_H_
