//
// Created by kanawo on 2023/2/24.
//
#include "fdel.h"
#include <iostream>
#include <error.h>
#include <cstring>


void fdel::Parse(const std::string &type, const std::string &msg)
{
    std::string filename = std::string(msg, msg.find_last_of(' ') + 1);
    filename = std::string(filename, 0, filename.size() - 2);
    filename = cmdTask->curDir + "/" + filename;
    if(remove(filename.c_str()) < 0)
    {
        std::cout << strerror(errno) << std::endl;
        ResCMD("502 file delete failed\r\n");
        return;
    }
    ResCMD("226 file delete success\r\n");
}