#pragma once
#include "EventLoop.h"
#include <sys/epoll.h> //仅用于包含 EPOLLIN 和 EPOLLOUT 宏
#include <functional>

class EventLoop;

class Channel
{
     std::function<void()> readCallback_, writeCallback_, errorCallback_; // 回调函数
     EventLoop *loop_;                                                    // Channel对应的EventLoop
     int fd_;                                                             // Channel对应的fd
     uint32_t events_ = 0;                                                // 订阅的事件
     uint32_t revents_;                                                   // 实际发生的事件
     bool isInEpoll_ = false;                                             // 是否已经注册到Epoll中
     void update();

public:
     Channel(EventLoop *loop, int fd);
     ~Channel() {}

     void setReadCallback(const std::function<void()> &cb) { readCallback_ = cb; }
     void setWriteCallback(const std::function<void()> &cb) { writeCallback_ = cb; }
     void setErrorCallback(const std::function<void()> &cb) { errorCallback_ = cb; }

     void setRevents(uint32_t revents) { revents_ = revents; }
     uint32_t getEvents() { return events_; }
     int getFd() { return fd_; }

     bool isInEpoll() { return isInEpoll_; }
     void setIsInEpoll(bool isInEpoll) { isInEpoll_ = isInEpoll; }
     void remove();

     void handleEvent(); // 事件触发时的回调处理

     void enableReading()
     {
          events_ |= EPOLLIN;
          update();
     }
     void enableWriting()
     {
          events_ |= EPOLLOUT;
          update();
     };
     void disableReading()
     {
          events_ &= ~EPOLLIN;
          update();
     }
     void disableWriting()
     {
          events_ &= ~EPOLLOUT;
          update();
     }
     void disableAll()
     {
          events_ = 0;
          update();
     }
};