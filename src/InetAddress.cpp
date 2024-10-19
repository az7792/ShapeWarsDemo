#include "InetAddress.h"

InetAddress::InetAddress(std::string ip, uint16_t port)
{
     addr_ = {0};
     addr_.sin_family = AF_INET;
     addr_.sin_port = htons(port);
     addr_.sin_addr.s_addr = inet_addr(ip.c_str());
     if (addr_.sin_addr.s_addr == INADDR_NONE)
     {
          Logger::instance().error("无效IP地址:" + ip);
          throw std::invalid_argument("Invalid IP address");
     }
}

InetAddress::InetAddress(const sockaddr_in &addr) : addr_(addr) {}

std::string InetAddress::getIp() const { return inet_ntoa(addr_.sin_addr); }

uint16_t InetAddress::getPort() const { return ntohs(addr_.sin_port); }

std::string InetAddress::getIpPort() const { return getIp() + ":" + std::to_string(getPort()); }

sockaddr_in *InetAddress::getSockaddr() { return &addr_; }

void InetAddress::setSockaddr(const sockaddr_in &addr) { addr_ = addr; }
