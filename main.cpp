#include <iostream>
#include "include/Time.h"
#include <unistd.h>
#include "include/Logger.h"
int main()
{
     Time tm = Time::now();
     std::cout << tm.toString() << " " << tm.getSecond();
     sleep(1);
     tm.setTimestamp(Time::now().getTimestamp());
     std::cout << tm.toString() << " " << tm.getSecond();
     Logger::instance().setLevel(LogLevel::FATAL);
     Logger::instance().fatal("fa");
     return 0;
}