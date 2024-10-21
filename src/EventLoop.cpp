#include "EventLoop.h"

EventLoop::EventLoop() : epoll_(new Epoll(this)), running_(false)
{
}

EventLoop::~EventLoop()
{
     delete epoll_;
}

void EventLoop::loop()
{
     running_ = true;
     Logger::instance().info("开启事件循环");
     while (running_)
     {
          std::vector<Channel *> chs = epoll_->wait();
          for (auto &ch : chs)
               ch->handleEvent();
     }
}

void EventLoop::quit()
{
     running_ = false;
}

void EventLoop::updateChannel(Channel *ch)
{
     if (ch->isInEpoll())
          epoll_->updateChannel(EPOLL_CTL_MOD, ch); // 在就修改
     else
          epoll_->updateChannel(EPOLL_CTL_ADD, ch); // 不在就ADD
}

void EventLoop::removeChannel(Channel *ch)
{
     epoll_->removeChannel(ch);
}