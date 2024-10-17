#pragma once
#include <string>
#include <ctime>
#include <sstream>
class Time
{
     std::time_t second_;

public:
     Time();
     explicit Time(std::time_t second);
     static Time now();
     std::string toString() const;
};