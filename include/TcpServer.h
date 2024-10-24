#pragma once
#include "EventLoop.h"
#include "ThreadPool.h"
#include "Socket.h"
#include "Acceptor.h"
#include "TcpConnection.h"
#include <vector>
#include <atomic>
class TcpServer
{
     std::atomic<int> numSubLoop_;
     std::atomic<int> nowIndex_ = -1;
     InetAddress localAddr_;
     EventLoop mainLoop_;
     Acceptor acceptor_;
     ThreadPool threadPool_;
     std::vector<EventLoop> subLoops_;
     int nextIndex();
     std::function<void(TcpConnection *)> onMessageCallback_, newConnectionCallback_;

public:
     TcpServer(const InetAddress &addr, int numSubLoop = 16);
     ~TcpServer();
     void setNewConnectionCallback(std::function<void(TcpConnection *)> cb) { newConnectionCallback_ = cb; }
     void setOnMessageCallback(std::function<void(TcpConnection *)> cb) { onMessageCallback_ = cb; }
     void newConnection(int sockfd, const InetAddress &addr);
     void run();
};