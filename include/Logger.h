#pragma once
#include "Time.h"
#include <mutex>
#include <filesystem>
#include <fstream>
#include <vector>
#include <cassert>
#include <iostream>
// 定义日志的级别
enum class LogLevel
{
     DEBUG, // 0 用于开发和调试阶段，记录详细的信息。
     INFO,  // 1 用于记录正常运行的状态和事件。
     WARN,  // 2 用于警告潜在的问题，程序仍然可以继续运行。
     ERROR, // 3 用于记录错误信息，表明发生了问题，但程序仍然可以继续运行。
     FATAL, // 4 用于记录严重错误，程序可能无法继续运行。
};
class Logger
{

     const std::vector<std::string> logLevelStr_ = {"DEBUG", "INFO", "WARN", "ERROR", "FATAL"};
     LogLevel logLevel_ = LogLevel::INFO;

     std::mutex mutex_;

     int year_ = 0, month_ = 0, day_ = 0; // 最近一次写日志的年月日
     std::string logFilePath_;            // 日志文件路径
     std::ofstream logFile_;              // 日志文件流
     void updateLogFile(const Time &tm);  // 更新日志文件

     void log(const Time &tm, LogLevel logLevel, const std::string &message); // 输出日志

     Logger();

public:
     ~Logger();
     Logger(const Logger &other) = delete;
     Logger &operator=(const Logger &other) = delete;

     /// @brief 获取Logger
     /// @return 返回Logger实例
     static Logger &instance();

     /**
      * @brief 设置日志等级
      *
      * @param logLevel 日志等级 [DEBUG, INFO, WARN, ERROR, FATAL]
      *
      * 设置日志等级时，只有高于或等于所设置的日志级别的日志消息才会被记录和输出：
      * - DEBUG：记录所有级别的日志消息。
      * - INFO：只记录 INFO、WARN、ERROR 和 FATAL 级别的日志消息。
      * - WARN：只记录 WARN、ERROR 和 FATAL 级别的日志消息。
      * - ERROR：只记录 ERROR 和 FATAL 级别的日志消息。
      * - FATAL：只记录 FATAL 级别的日志消息。
      */
     void setLevel(LogLevel logLevel); // 设置日志级别

     // 输出日志
     void debug(const std::string &message);
     void info(const std::string &message);
     void warn(const std::string &message);
     void error(const std::string &message);
     void fatal(const std::string &message);
};