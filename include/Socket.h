#pragma once
#include "Logger.h"
#include "InetAddress.h"
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/tcp.h>
#include <cstring>
#include <fcntl.h>

class Socket
{
     int fd_ = -1;

public:
     Socket(int fd) : fd_(fd) {}
     ~Socket();

     int getFd() const { return fd_; };

     void bind(const InetAddress &addr);
     void listen(int __n = 128);
     int accept(InetAddress *addr);         // 默认返回非阻塞IO
     void connect(const InetAddress &addr); // 连接到指定服务器

     ssize_t send(const std::string mes);
     std::string recv();

     void shutdownWrite();

     void setTcpNoDelay(bool on); // 设置是否禁用 Nagle 算法。 true为关闭 false为开启
     void setReuseAddr(bool on);  // 设置是否允许地址重用
     void setReusePort(bool on);  // 设置是否允许端口重用
     void setKeepAlive(bool on);  // 设置是否启用 TCP 保活机制
     void setBlocking(bool on);   // 设置是否启用阻塞 I/O，true 为启用，false 为非阻塞
};