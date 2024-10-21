#pragma once
#include <sys/epoll.h>
#include <unistd.h>
#include <cstring>
#include <map>
#include <vector>
#include "EventLoop.h"
#include "Channel.h"
#include "Logger.h"

class Channel;
class EventLoop;

class Epoll
{
     int epollFd_;
     EventLoop *loop_;
     std::map<int, Channel *> channelMap_; // 管理所有Channel
     std::vector<epoll_event> events_;     // 存放epoll_wait返回的事件
     int __maxevents = 16;                 // epoll_wait返回的事件的最大长度

public:
     Epoll(EventLoop *loop);
     ~Epoll();
     std::vector<Channel *> wait(int timeout = -1);
     void updateChannel(int op,Channel *ch);
     void removeChannel(Channel *ch);
};