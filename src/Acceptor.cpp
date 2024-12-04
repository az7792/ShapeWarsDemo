#include "Acceptor.h"

Acceptor::Acceptor(EventLoop *loop, const InetAddress &listenAddr) : mainLoop_(loop), acceptSocket_(newSocket()),
                                                                     acceptChannel_(loop, acceptSocket_.getFd())
{
     acceptSocket_.setReuseAddr(true);
     acceptSocket_.setReusePort(true);
     acceptSocket_.bind(listenAddr);
     acceptChannel_.enableReading();
     acceptChannel_.setReadCallback(std::bind(&Acceptor::handleRead, this));
     acceptSocket_.listen(128);
}

Acceptor::~Acceptor()
{
     acceptChannel_.remove();
}

void Acceptor::handleRead()
{
     InetAddress addr;
     int cfd = acceptSocket_.accept(&addr);
     newConnectionCallback_(cfd, addr);
}

int Acceptor::newSocket()
{
     int fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, IPPROTO_TCP);
     if (fd < 0)
          Logger::instance().fatal("Acceptor初始化失败:" + std::string(strerror(errno)));
     return fd;
}
