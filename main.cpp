#include <iostream>
#include <unistd.h>
#include "include/Time.h"
#include "include/Logger.h"
#include "include/InetAddress.h"
using namespace std;
int main()
{
     Logger::instance().setLevel(LogLevel::DEBUG);
     InetAddress addr("123.56.155.4", 7792);
     cout << addr.getIpPort();
     cout << endl;
     return 0;
}