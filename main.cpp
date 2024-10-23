#include <iostream>
#include <unistd.h>
#include "include/Time.h"
#include "include/Logger.h"
#include "include/InetAddress.h"
#include "include/Socket.h"
#include "include/EventLoop.h"
#include "include/Acceptor.h"
#include "include/TcpConnection.h"
using namespace std;
InetAddress localAddr("127.0.0.1", 7792);
EventLoop loop;
void readCb(TcpConnection *tc)
{
     Socket *sc = tc->getSocket();
     string str = sc->recv();
     if (str == "")
     {
          Logger::instance().info(tc->getPeerAddr().getIpPort() + "断开连接");
          tc->close();
          return;
     }
     cout << str << endl;
     tc->send("~" + str + "~");
}
vector<TcpConnection *> tcps;
void connection(int cfd, const InetAddress &addr)
{
     Logger::instance().info(addr.getIpPort() + "已连接");
     TcpConnection *tc = new TcpConnection(&loop, localAddr, addr, cfd);
     tcps.push_back(tc);
     (*tc).setReadCallback(std::bind(readCb, tc));
}
int main()
{
     Logger::instance().setLevel(LogLevel::DEBUG);
     Acceptor ac(&loop, localAddr);
     ac.setNewConnectionCallback(std::function<void(int fd, const InetAddress &addr)>(connection));

     loop.loop();

     for (auto v : tcps)
          delete v;
     return 0;
}