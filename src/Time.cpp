#include "Time.h"

Time::Time() : second_(0) {}

Time::Time(std::time_t second) : second_(second) {}

Time Time::now() { return Time(std::time(nullptr)); }

std::string Time::toString() const
{
     // 指向一个静态分配的 struct tm,每次调用会覆盖这个 tm ,不需要手动释放
     std::tm *localTime = std::localtime(&second_);

     std::ostringstream oss;
     oss << (localTime->tm_year + 1900) << "-"
         << (localTime->tm_mon + 1) << "-"
         << localTime->tm_mday << " "
         << localTime->tm_hour << ":"
         << localTime->tm_min << ":"
         << localTime->tm_sec;

     return oss.str();
}