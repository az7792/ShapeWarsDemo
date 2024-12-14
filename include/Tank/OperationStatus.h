#pragma once
#include <cstdint>
#include <string>
#include <cstring>
enum KeyStatus
{
     LEFT = 0,  // 左键
     RIGHT = 1, // 右键
     W = 2,     // W
     A = 3,     // A
     S = 4,     // S
     D = 5      // D
};
struct OperationStatus
{
     float x = 0, y = 0;
     uint64_t keyStatus = 0;
     void setKeyStatus(int p, bool v)
     {
          if (0 <= p && p < 64)
               keyStatus = v ? (keyStatus | (1ULL << p)) : (keyStatus & ~(1ULL << p));
     }
     bool getKeyStatus(int p)
     {
          return ((keyStatus >> p) & 1ULL);
     }
     void update(std::string msg)
     {
          int offset = 0;
          std::memcpy(&x, msg.data() + offset, 4);
          offset += 4;
          std::memcpy(&y, msg.data() + offset, 4);
          offset += 4;
          std::memcpy(&keyStatus, msg.data() + offset, 8);
          offset += 8;
     }
};
