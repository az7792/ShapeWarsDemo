#include "Socket.h"
#include <iostream>
#include <cstring>
Socket::~Socket()
{
     if (fd_ != -1)
     {
          ::close(fd_);
          fd_ = -1;
     }
}

void Socket::bind(const InetAddress &addr)
{
     if (0 != ::bind(fd_, (sockaddr *)addr.getSockaddr(), sizeof(sockaddr_in)))
          Logger::instance().fatal("sockfd" + std::to_string(fd_) + " 绑定失败: " + strerror(errno));
}

void Socket::listen(int __n)
{
     if (0 != ::listen(fd_, __n))
          Logger::instance().fatal("sockfd" + std::to_string(fd_) + " 监听失败: " + strerror(errno));
}

int Socket::accept(InetAddress *addr)
{
     sockaddr_in addr_in = {0};
     socklen_t len = sizeof(sockaddr_in);
     int cfd = ::accept4(fd_, (sockaddr *)&addr_in, &len, SOCK_NONBLOCK | SOCK_CLOEXEC);
     if (cfd >= 0)
          addr->setSockaddr(addr_in);
     else
          Logger::instance().error("sockfd" + std::to_string(fd_) + " accept 失败: " + strerror(errno));
     return cfd;
}

void Socket::connect(const InetAddress &addr)
{
     if (::connect(fd_, (sockaddr *)addr.getSockaddr(), sizeof(sockaddr_in)) < 0)
     {
          Logger::instance().fatal("sockfd" + std::to_string(fd_) + " 连接失败: " + strerror(errno));
          throw std::runtime_error("Connection failed");
     }
}

ssize_t Socket::send(const std::string &mes)
{
     ssize_t sentBytes = ::send(fd_, mes.c_str(), mes.size(), 0);
     if (sentBytes < 0)
     {
          if (errno == EPIPE)
               Logger::instance().warn("socketfd:" + std::to_string(fd_) + " 发送失败: 对端已关闭");
          else
               Logger::instance().error("socketfd:" + std::to_string(fd_) + " 发送失败: " + strerror(errno));
     }
     return sentBytes;
}

std::string Socket::recv()
{
     char buffer[16384]; // 16k
     // char buffer[65536]; // 64k
     ssize_t receivedBytes = ::recv(fd_, buffer, sizeof(buffer) - 1, 0);

     if (receivedBytes < 0)
     {
          // if (errno == EAGAIN || errno == EWOULDBLOCK)
          // {
          //      // 非阻塞模式下可能没有数据可读，并不一定读取错误
          //      return "";
          // }
          Logger::instance().error("socketfd:" + std::to_string(fd_) + " 接收失败: " + strerror(errno));
          // 异常断开
          return "";
     }
     // 正常连接断开这儿返回的是空字符串
     return std::string(buffer, receivedBytes);
}

void Socket::shutdownWrite()
{
     if (::shutdown(fd_, SHUT_WR) < 0)
          Logger::instance().warn("sockfd" + std::to_string(fd_) + " 写端关闭失败: " + strerror(errno));
}

void Socket::setTcpNoDelay(bool on)
{
     int opt = on ? 1 : 0;
     if (setsockopt(fd_, IPPROTO_TCP, TCP_NODELAY, (const char *)&opt, sizeof(opt)) < 0)
          Logger::instance().warn("sockfd" + std::to_string(fd_) + " TCP_NODELAY 设置失败: " + strerror(errno));
}

void Socket::setReuseAddr(bool on)
{
     int opt = on ? 1 : 0;
     if (setsockopt(fd_, SOL_SOCKET, SO_REUSEADDR, (const char *)&opt, sizeof(opt)) < 0)
          Logger::instance().warn("sockfd" + std::to_string(fd_) + " SO_REUSEADDR 设置失败: " + strerror(errno));
}

void Socket::setReusePort(bool on)
{
     int opt = on ? 1 : 0;
     if (setsockopt(fd_, SOL_SOCKET, SO_REUSEPORT, (const char *)&opt, sizeof(opt)) < 0)
          Logger::instance().warn("sockfd" + std::to_string(fd_) + " SO_REUSEPORT 设置失败: " + strerror(errno));
}

void Socket::setKeepAlive(bool on)
{
     int opt = on ? 1 : 0;
     if (setsockopt(fd_, SOL_SOCKET, SO_KEEPALIVE, (const char *)&opt, sizeof(opt)) < 0)
          Logger::instance().warn("sockfd" + std::to_string(fd_) + " SO_KEEPALIVE 设置失败: " + strerror(errno));
}

void Socket::setBlocking(bool on)
{
     int flags = fcntl(fd_, F_GETFL, 0);
     if (flags == -1)
     {
          Logger::instance().error("获取文件状态标志失败");
          return;
     }

     if (on)
          flags &= ~O_NONBLOCK; // 阻塞
     else
          flags |= O_NONBLOCK; // 非阻塞

     if (fcntl(fd_, F_SETFL, flags) == -1)
          Logger::instance().error("设置文件状态标志失败");
}
