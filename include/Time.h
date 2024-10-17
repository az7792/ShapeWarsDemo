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

     // 获取时间戳
     std::time_t getTime_t() const;
     
     //  获取年
     int getYear() const;

     // 获取月
     int getMonth() const;

     // 获取日
     int getDay() const;

     // 获取时
     int getHour() const;

     // 获取分
     int getMinute() const;

     // 获取秒
     int getSecond() const;
};