#include <iostream>
#include <unistd.h>
#include "include/Time.h"
#include "include/Logger.h"
#include "include/InetAddress.h"
#include "include/Socket.h"
#include "include/EventLoop.h"
using namespace std;
Socket *sfd;
EventLoop loop;
vector<Channel *> chs;
vector<Socket *> socks;
void readCb(Socket *sc, Channel *ch)
{
     string sss = sc->recv();
     if (sss == "")
     {
          loop.removeChannel(ch);
          Logger::instance().info("exit");
          return;
     }
     cout << "客户端:" << sss << endl;
     sc->send("echo" + sss);
}
void accept_(InetAddress *addr)
{
     int fd = sfd->accept(addr);
     Logger::instance().info(addr->getIpPort() + "已连接");
     Socket *sc = new Socket(fd);
     sc->setBlocking(true);
     Channel *ch = new Channel(&loop, fd);
     chs.push_back(ch);
     socks.push_back(sc);
     (*ch).enableReading();
     (*ch).setReadCallback(std::bind(readCb, sc, ch));
     loop.updateChannel(ch);
}
int main()
{
     Logger::instance().setLevel(LogLevel::DEBUG);
     int tmpSfd = socket(AF_INET, SOCK_STREAM, 0);
     sfd = new Socket(tmpSfd);
     sfd->bind(InetAddress("127.0.0.1", 7792));
     sfd->listen();
     sfd->setBlocking(true);
     Channel ch(&loop, tmpSfd);
     ch.enableReading();
     InetAddress addr;
     ch.setReadCallback(std::bind(accept_, &addr));
     loop.updateChannel(&ch);

     loop.loop();

     for (auto v : chs)
          delete v;
     for (auto v : socks)
          delete v;
     return 0;
}