#include <iostream>
#include "Socket.h"
#include <string>
using namespace std;
int main()
{
     int tmpCfd = socket(AF_INET, SOCK_STREAM, 0);
     Socket cfd(tmpCfd);
     cfd.connect(InetAddress("127.0.0.1", 7792));
     string str;
     while (true)
     {
          cin >> str;
          if (str == "exit")
               break;
          cfd.send(str);
          string recvStr = cfd.recv();
          if (recvStr.empty())
          {
               cout << "服务器断开\n";
               break;
          }
          cout << "server:" << recvStr << endl;
     }
     return 0;
}