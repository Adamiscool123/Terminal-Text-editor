// Declares text-editing commands invoked from Setup::edit (e.g., insert mode).
#ifndef _COMMANDS_H_
#define _COMMANDS_H_

#include <iostream>
#include <fstream>

class Commands{
public:
    void i(std::fstream&);       // “insert-mode” entry point; receives a stream
};

#endif //_COMMANDS_H_
