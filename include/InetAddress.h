#pragma once
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string>
#include "Logger.h"

class InetAddress
{
     sockaddr_in addr_;

public:
     InetAddress(std::string ip = "127.0.0.1", uint16_t port = 0);
     InetAddress(const sockaddr_in &addr);

     std::string getIp() const;
     uint16_t getPort() const;
     std::string getIpPort() const;

     const sockaddr_in *getSockaddr() const;
     void setSockaddr(const sockaddr_in &addr);
};