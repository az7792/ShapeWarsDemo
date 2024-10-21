#pragma once
#include "Channel.h"
#include "Epoll.h"

class Channel;
class Epoll;

class EventLoop
{
     Epoll *epoll_;
     bool running_;

public:
     EventLoop();
     ~EventLoop();
     
     // 开启事件循环
     void loop();
     // 退出事件循环
     void quit();

     void updateChannel(Channel *ch);
     void removeChannel(Channel *ch);
};