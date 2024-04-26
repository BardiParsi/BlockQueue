#pragma once
#include <iostream>
#include <mutex>
#include <string>
#include <sstream>

using std::cout;
using std::endl;

//Logger class is to help the cout output function to console to be organized and clean by using special output mutex.

class Logger
{
public:
    static std::mutex logMtx;
    static void INFO(const std::string& message);
};
