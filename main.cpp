#include <iostream>
#include <unistd.h>
#include "include/Time.h"
#include "include/Logger.h"
#include "include/InetAddress.h"
#include "include/Socket.h"
#include "include/EventLoop.h"
#include "include/Acceptor.h"
using namespace std;
EventLoop loop;
vector<Channel *> chs;
void readCb(int fd, Channel *ch)
{
     char buf[1024];
     recv(fd, buf, 1024, 0);
     if (strcmp(buf, "") == 0)
     {
          cout << "exit\n";
          ch->remove();
          return;
     }
     cout << buf << endl;
     send(fd, buf, strlen(buf), 0);
     bzero(buf, 1024);
}
void connection(int cfd, const InetAddress &addr)
{
     Logger::instance().info(addr.getIpPort() + "已连接");
     Channel *ch = new Channel(&loop, cfd);
     chs.push_back(ch);
     (*ch).enableReading();
     (*ch).setReadCallback(std::bind(readCb, cfd, ch));
}
int main()
{
     Logger::instance().setLevel(LogLevel::DEBUG);
     InetAddress addr("127.0.0.1", 7792);
     Acceptor ac(&loop, addr);
     ac.setNewConnectionCallback(std::function<void(int fd, const InetAddress &addr)>(connection));

     loop.loop();

     for (auto v : chs)
          delete v;
     return 0;
}