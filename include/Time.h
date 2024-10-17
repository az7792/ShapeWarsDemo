#pragma once
#include <string>
#include <ctime>
#include <sstream>
#include <iomanip>
class Time
{
     std::time_t timestamp_;
     int year_, month_, day_, hour_, minute_, second_;

public:
     Time();
     explicit Time(std::time_t timestamp);
     static Time now();
     std::string toString() const;

     // 获取时间戳
     std::time_t getTimestamp() const;

     // 设置时间戳
     void setTimestamp(std::time_t timestamp);

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