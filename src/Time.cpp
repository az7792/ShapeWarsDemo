#include "Time.h"

Time::Time() : timestamp_(0), year_(1970), month_(1), day_(1), hour_(0), minute_(0), second_(0) {}

Time::Time(std::time_t timestamp) : timestamp_(timestamp) { setTimestamp(timestamp_); }

Time Time::now() { return Time(std::time(nullptr)); }

std::string Time::toString() const
{
    std::ostringstream oss;
    oss << std::setw(4) << std::setfill('0') << year_ << "-"
        << std::setw(2) << std::setfill('0') << month_ << "-"
        << std::setw(2) << std::setfill('0') << day_ << " "
        << std::setw(2) << std::setfill('0') << hour_ << ":"
        << std::setw(2) << std::setfill('0') << minute_ << ":"
        << std::setw(2) << std::setfill('0') << second_;
    return oss.str();
}

// 获取时间戳
std::time_t Time::getTimestamp() const { return timestamp_; }

// 设置时间戳
void Time::setTimestamp(std::time_t timestamp)
{
    timestamp_ = timestamp;

    std::tm localTime;
    localtime_r(&timestamp_, &localTime);

    year_ = (localTime.tm_year + 1900);
    month_ = (localTime.tm_mon + 1);
    day_ = localTime.tm_mday;
    hour_ = localTime.tm_hour;
    minute_ = localTime.tm_min;
    second_ = localTime.tm_sec;
}

// 获取年
int Time::getYear() const { return year_; }

// 获取月
int Time::getMonth() const { return month_; }

// 获取日
int Time::getDay() const { return day_; }

// 获取时
int Time::getHour() const { return hour_; }

// 获取分
int Time::getMinute() const { return minute_; }

// 获取秒
int Time::getSecond() const { return second_; }