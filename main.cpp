#include <iostream>
#include <unistd.h>
#include "include/Time.h"
#include "include/Logger.h"
#include "include/InetAddress.h"
#include "include/Socket.h"
#include "include/EventLoop.h"
#include "include/Acceptor.h"
#include "include/TcpConnection.h"
#include "include/ThreadPool.h"
#include "include/TcpServer.h"
using namespace std;

void readCb(TcpConnection *tc)
{
     Socket *sc = tc->getSocket();
     string str = sc->recv();
     if (str == "")
     {
          Logger::instance().info(tc->getPeerAddr().getIpPort() + "断开连接");
          delete tc; // 连接断开后直接delete就行
          return;
     }
     if (str == "sleep")
     {
          std::this_thread::sleep_for(std::chrono::seconds(10));
     }
     cout << str << endl;
     tc->send("~" + str + "~");
}

void connection(TcpConnection *tc) // tc是已经连接的链接
{
     Logger::instance().info(tc->getPeerAddr().getIpPort() + "已连接");
}
int main()
{
     Logger::instance().setLevel(LogLevel::DEBUG);
     TcpServer server(InetAddress("127.0.0.1", 7792), 2);
     server.setNewConnectionCallback(connection);
     server.setOnMessageCallback(readCb);
//   std::this_thread::sleep_for(std::chrono::seconds(1));
     server.run();
     return 0;
}