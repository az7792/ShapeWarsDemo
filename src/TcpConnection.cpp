#include "TcpConnection.h"

TcpConnection::TcpConnection(EventLoop *loop, InetAddress localAddr, InetAddress peerAddr, int fd)
    : localAddr_(localAddr), peerAddr_(peerAddr),
      loop_(loop),
      sock_(new Socket(fd)), channel_(new Channel(loop, fd)),
      status_(TcpConnectionStatus::Disconnected)
{
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
     return sock_->send(buf);
}

std::string TcpConnection::recv()
{
     return std::move(sock_->recv());
}

void TcpConnection::close()
{
     delete channel_;
     delete sock_;
     channel_ = nullptr;
     sock_ = nullptr;
     setStatus(TcpConnectionStatus::Disconnected);
}
