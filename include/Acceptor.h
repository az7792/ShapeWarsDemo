#pragma once
#include "EventLoop.h"
#include "Socket.h"
#include "Channel.h"

class Acceptor
{
     EventLoop *mainLoop_;
     Socket acceptSocket_;
     Channel acceptChannel_;
     std::function<void(int sockfd, const InetAddress &addr)> newConnectionCallback_;
     int nowSocket();

public:
     Acceptor(EventLoop *loop, const InetAddress &listenAddr);
     ~Acceptor();
     void setNewConnectionCallback(std::function<void(int sockfd, const InetAddress &addr)> cb) { newConnectionCallback_ = cb; }
     void handleRead();
};