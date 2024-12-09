#include "TcpConnection.h"

TcpConnection::TcpConnection(EventLoop *loop, InetAddress localAddr, InetAddress peerAddr, int fd)
    : localAddr_(localAddr), peerAddr_(peerAddr),
      loop_(loop),
      sock_(new Socket(fd)), channel_(new Channel(loop, fd)),
      status_(TcpConnectionStatus::Disconnected)
{
     sock_->setTcpNoDelay(true);
     channel_->enableReading();
     // channel_->enableWriting();
     channel_->setReadCallback(std::bind(&TcpConnection::handleRead, this));
     channel_->setWriteCallback(std::bind(&TcpConnection::handleWrite, this));
     channel_->setErrorCallback(std::bind(&TcpConnection::handleError, this));
     setStatus(TcpConnectionStatus::Connected);
}

TcpConnection::~TcpConnection()
{
     if (status_ == TcpConnectionStatus::Connected)
          close();
}

int TcpConnection::send(const std::string &buf)
{
     if (status_ == TcpConnectionStatus::Disconnected)
          return 0;
     int t = sock_->send(buf);
     if (t <= 0)
          close();
     return t;
}

std::string TcpConnection::recv()
{
     if (status_ == TcpConnectionStatus::Disconnected)
          return "";
     std::string t = sock_->recv();
     if (t.empty())
          close();
     return t;
}

void TcpConnection::close()
{
     delete channel_;
     delete sock_;
     channel_ = nullptr;
     sock_ = nullptr;
     setStatus(TcpConnectionStatus::Disconnected);
}
