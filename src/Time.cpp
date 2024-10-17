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

// 获取时间戳
std::time_t Time::getTime_t() const
{
    return second_;
}

// 获取年
int Time::getYear() const
{
    std::tm *localTime = std::localtime(&second_);
    return localTime->tm_year + 1900; // tm_year 是从 1900 年开始的年数
}

// 获取月
int Time::getMonth() const
{
    std::tm *localTime = std::localtime(&second_);
    return localTime->tm_mon + 1; // tm_mon 是从 0 开始的月份
}

// 获取日
int Time::getDay() const
{
    std::tm *localTime = std::localtime(&second_);
    return localTime->tm_mday;
}

// 获取时
int Time::getHour() const
{
    std::tm *localTime = std::localtime(&second_);
    return localTime->tm_hour;
}

// 获取分
int Time::getMinute() const
{
    std::tm *localTime = std::localtime(&second_);
    return localTime->tm_min;
}

// 获取秒
int Time::getSecond() const
{
    std::tm *localTime = std::localtime(&second_);
    return localTime->tm_sec;
}