#include "Channel.h"

Channel::Channel(EventLoop *loop, int fd):loop_(loop),fd_(fd){}

void Channel::update() { loop_->updateChannel(this); }

void Channel::remove() { loop_->removeChannel(this); }

void Channel::handleEvent() // 事件触发时的回调处理
{
     if ((revents_ & EPOLLERR) && errorCallback_)
          errorCallback_();
     if ((revents_ & EPOLLIN) && readCallback_)
          readCallback_();
     if ((revents_ & EPOLLOUT) && writeCallback_)
          writeCallback_();
}
