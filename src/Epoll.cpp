#include "Epoll.h"

Epoll::Epoll(EventLoop *loop) : epollFd_(epoll_create1(EPOLL_CLOEXEC)),
                                loop_(loop)
{
     if (epollFd_ < 0)
          Logger::instance().fatal("Epoll创建失败" + std::string(strerror(errno)));
     events_.resize(__maxevents);
}

Epoll::~Epoll()
{
     ::close(epollFd_);
}

std::vector<Channel *> Epoll::wait(int timeout)
{
     // events_.data()的使用长度最多不超过__maxevents
     int nfds = epoll_wait(epollFd_, events_.data(), __maxevents, timeout);
     std::vector<Channel *> chs;
     if (nfds > 0)
     {
          chs.reserve(nfds); // 预留空间
          for (int i = 0; i < nfds; ++i)
          {
               static_cast<Channel *>(events_[i].data.ptr)->setRevents(events_[i].events);
               chs.push_back(static_cast<Channel *>(events_[i].data.ptr));
          }
          if (__maxevents == nfds)
          {
               events_.resize(2 * __maxevents); // 借助vector进行扩容
          }
     }
     else if (nfds == 0)
     {
          Logger::instance().debug("epoll wait超时");
     }
     else
     {
          Logger::instance().error("Epoll wait失败" + std::string(strerror(errno)));
     }
     return chs;
}

void Epoll::updateChannel(int op, Channel *ch)
{
     epoll_event ev;
     ev.events = ch->getEvents();
     ev.data.ptr = ch;

     if (epoll_ctl(epollFd_, op, ch->getFd(), &ev) == -1)
          Logger::instance().error("epoll_ctl失败:" + std::string(strerror(errno)));

     if (op == EPOLL_CTL_ADD)
     {
          channelMap_.emplace(ch->getFd(), ch);
          ch->setIsInEpoll(true);
     }
     else if (op == EPOLL_CTL_DEL)
     {
          channelMap_.erase(ch->getFd());
          ch->setIsInEpoll(false);
     }
}

void Epoll::removeChannel(Channel *ch)
{
     if (ch->isInEpoll())
          updateChannel(EPOLL_CTL_DEL, ch);
}
