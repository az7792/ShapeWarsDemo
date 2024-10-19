#include <iostream>
#include <unistd.h>
#include "include/Time.h"
#include "include/Logger.h"
#include "include/InetAddress.h"
#include "include/Socket.h"
using namespace std;
int main()
{
     Logger::instance().setLevel(LogLevel::DEBUG);
     int tmpSfd = socket(AF_INET, SOCK_STREAM, 0);
     Socket sfd(tmpSfd);
     sfd.bind(InetAddress("127.0.0.1", 7792));
     sfd.listen();
     InetAddress cAddr;
     int tmpCfd = sfd.accept(&cAddr);
     Socket cfd(tmpCfd);
     cout << cAddr.getIpPort() << "连接成功\n";
     cfd.setBlocking(true);
     while (1)
     {
          string str = cfd.recv();
          if (str.empty())
               break;
          cout << "client:" << str << endl;
          cfd.send(str + "hahaha");
     }
     return 0;
}