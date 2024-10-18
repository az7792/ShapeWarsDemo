#include "Logger.h"

Logger::Logger() { updateLogFile(Time::now()); }

Logger::~Logger()
{
     if (logFile_.is_open())
     {
          logFile_.close();
     }
}

Logger &Logger::instance()
{
     static Logger logger;
     return logger;
}

// 更新文件路径
void Logger::updateLogFile(const Time &tm)
{
     int year = tm.getYear();
     int month = tm.getMonth();
     int day = tm.getDay();

     if (day == day_ && month == month_ && year == year_)
          return;

     year_ = year;
     month_ = month;
     day_ = day;

     std::ostringstream oss;
     oss << "logs/"
         << year_ << "/"                                         // 年
         << std::setw(2) << std::setfill('0') << month_ << "/"   // 月
         << std::setw(2) << std::setfill('0') << day_ << ".log"; // 日

     // 创建目录结构
     std::filesystem::create_directories(oss.str().substr(0, oss.str().find_last_of('/'))); // 创建年和月目录

     logFilePath_ = oss.str();

     // 确保更新后是最新的
     if (logFile_.is_open())
          logFile_.close();
     logFile_.open(logFilePath_, std::ios::app);
}

void Logger::setLevel(LogLevel logLevel) { logLevel_ = logLevel; }

void Logger::log(const Time &tm, LogLevel logLevel, const std::string &message)
{
     if (static_cast<int>(logLevel) < static_cast<int>(logLevel_))
          return;
     // 写入日志，最多尝试 2 次
     for (int i = 0; i < 2; ++i)
     {
          updateLogFile(tm);
          if (logFile_.is_open())
          {
               //[2024-10-18 11:24:45] [INFO] test
               logFile_ << "[" << tm.toString() << "] [" << logLevelStr_[static_cast<int>(logLevel)] << "] " << message << std::endl;
               break;
          }
          else if (i == 1)
          {
               std::cerr << "日志文件: " << logFilePath_ << " 打开失败!" << std::endl;
          }
     }
}

void Logger::debug(const std::string &message)
{
     std::lock_guard<std::mutex> lock(mutex_);
     log(Time::now(), LogLevel::DEBUG, message);
}

void Logger::info(const std::string &message)
{
     std::lock_guard<std::mutex> lock(mutex_);
     log(Time::now(), LogLevel::INFO, message);
}

void Logger::warn(const std::string &message)
{
     std::lock_guard<std::mutex> lock(mutex_);
     log(Time::now(), LogLevel::WARN, message);
}

void Logger::error(const std::string &message)
{
     std::lock_guard<std::mutex> lock(mutex_);
     log(Time::now(), LogLevel::ERROR, message);
}

void Logger::fatal(const std::string &message)
{
     std::lock_guard<std::mutex> lock(mutex_);
     log(Time::now(), LogLevel::FATAL, message);
}
