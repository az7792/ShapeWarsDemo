#include "TcpServer.h"

int TcpServer::nextIndex()
{
     ++nowIndex_;
     if (nowIndex_ == numSubLoop_)
          nowIndex_ = 0;
     return nowIndex_;
}

TcpServer::TcpServer(const InetAddress &addr, int numSubLoop)
    : numSubLoop_(numSubLoop), localAddr_(addr),
      mainLoop_(), acceptor_(&mainLoop_, localAddr_),
      threadPool_(numSubLoop_), subLoops_(numSubLoop_)
{
     acceptor_.setNewConnectionCallback(
         std::bind(&TcpServer::newConnection, this, std::placeholders::_1, std::placeholders::_2));
}

TcpServer::~TcpServer()
{
     mainLoop_.quit();
     for (auto &v : subLoops_)
          v.quit();
     threadPool_.close();
}

void TcpServer::newConnection(int sockfd, const InetAddress &addr) // 建立新连接
{
     int nextLoopIndex = nextIndex(); // 子eventloop
     TcpConnection *newTcpConnection = new TcpConnection(&subLoops_[nextLoopIndex], localAddr_, addr, sockfd);
     newTcpConnection->setReadCallback(std::bind(onMessageCallback_, newTcpConnection));
     if (newConnectionCallback_)
          newConnectionCallback_(newTcpConnection);
}

void TcpServer::run()
{
     for (auto &v : subLoops_)
     {
          threadPool_.submit(&EventLoop::loop, &v);
     }
     Logger::instance().debug("subLoop启动");
     // 最后开启mainLoop,mainLoop运行在当前线程，先开启会阻塞在wait上，导致之后的subLoop无法开启
     mainLoop_.loop();
}
