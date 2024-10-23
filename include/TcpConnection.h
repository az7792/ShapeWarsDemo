#pragma once
#include "Channel.h"
#include "InetAddress.h"
#include "EventLoop.h"
#include "Socket.h"
enum class TcpConnectionStatus
{
     Disconnected,
     Connected,
};
class TcpConnection
{
     InetAddress localAddr_;
     InetAddress peerAddr_;

     EventLoop *loop_;
     Socket *sock_;
     Channel *channel_;

     TcpConnectionStatus status_;

     std::function<void()> readCallback_, writeCallback_, errorCallback_; // 回调函数

     void handleRead() { readCallback_(); }
     void handleWrite() { writeCallback_(); }
     void handleError() { errorCallback_(); }
     void setStatus(TcpConnectionStatus status) { status_ = status; }

public:
     TcpConnection(EventLoop *loop, InetAddress localAddr, InetAddress peerAddr, int fd);
     ~TcpConnection();

     int send(const std::string &buf);
     TcpConnectionStatus getStatus() { return status_; }
     void setReadCallback(const std::function<void()> &cb) { readCallback_ = cb; }
     void setWriteCallback(const std::function<void()> &cb) { writeCallback_ = cb; }
     void setErrorCallback(const std::function<void()> &cb) { errorCallback_ = cb; }
     Socket *getSocket() { return sock_; }
     Channel *getChannel() { return channel_; }
     EventLoop *getEventLoop() { return loop_; }
     const InetAddress &getLocalAddr() { return localAddr_; }
     const InetAddress &getPeerAddr() { return peerAddr_; }
     void close(); // 断开连接
};