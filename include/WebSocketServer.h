#pragma once
#include <cstdint>
#include <stdexcept>
#include <cstring>
#include <functional>
#include <map>
#include "TcpServer.h"
class WebSocketFrame
{
public:
     bool fin;                        // FIN 位
     uint8_t opcode;                  // Opcode
     bool masked;                     // 是否启用 Mask
     uint64_t payloadLength;          // Payload 长度
     std::vector<uint8_t> maskingKey; // Masking Key（如果有）
     std::string payloadData;         // 实际数据

     bool ok = true; // 帧是否解析成功

     // 解析帧
     WebSocketFrame(const std::string &frameData)
     {
          parse(frameData);
     }

     // 生成 WebSocket 帧
     static std::string encode(bool fin, uint8_t opcode, bool masked, const std::string &payloadData)
     {
          std::string frame;

          // 第 1 字节：FIN + RSV + Opcode
          uint8_t firstByte = (fin ? 0x80 : 0x00) | (opcode & 0x0F);
          frame.push_back(firstByte);

          // 第 2 字节：Mask 位 + Payload 长度
          uint8_t secondByte = masked ? 0x80 : 0x00;
          size_t payloadLength = payloadData.size();

          if (payloadLength <= 125)
          {
               secondByte |= static_cast<uint8_t>(payloadLength);
               frame.push_back(secondByte);
          }
          else if (payloadLength <= 0xFFFF)
          {
               secondByte |= 126;
               frame.push_back(secondByte);
               frame.push_back((payloadLength >> 8) & 0xFF);
               frame.push_back(payloadLength & 0xFF);
          }
          else
          {
               secondByte |= 127;
               frame.push_back(secondByte);
               for (int i = 7; i >= 0; --i)
               {
                    frame.push_back((payloadLength >> (i * 8)) & 0xFF);
               }
          }

          // Masking Key
          if (masked)
          {
               std::vector<uint8_t> maskingKey = {0x12, 0x34, 0x56, 0x78}; // Masking Key
               frame.insert(frame.end(), maskingKey.begin(), maskingKey.end());

               // 应用 Mask 到 Payload 数据
               std::string maskedPayload = payloadData;
               for (size_t i = 0; i < payloadLength; ++i)
               {
                    maskedPayload[i] ^= maskingKey[i % 4];
               }
               frame += maskedPayload;
          }
          else
          {
               // 不加掩码，直接添加 Payload 数据
               frame += payloadData;
          }

          return frame;
     }

private:
     // 解析帧
     void parse(const std::string &frameData)
     {
          // 读取第 1 字节
          fin = (frameData[0] & 0x80) != 0; // FIN 位
          opcode = frameData[0] & 0x0F;     // Opcode
          if (fin == 1 && opcode == 0x0)
          {
               ok = false;
               Logger::instance().warn("FIN 与 opcode不匹配");
               return;
          }
          // 读取第 2 字节
          masked = (frameData[1] & 0x80) != 0;             // Mask 位
          uint8_t initialPayloadLen = frameData[1] & 0x7F; // Payload 长度（前7位）

          size_t index = 2; // 当前字节序号

          // 处理扩展长度
          if (initialPayloadLen == 126)
          {
               if (frameData.size() < index + 2)
               {
                    ok = false;
                    Logger::instance().error("扩展长度不足 2 字节");
                    return;
               }
               payloadLength = (static_cast<uint8_t>(frameData[index]) << 8) |
                               static_cast<uint8_t>(frameData[index + 1]);
               index += 2;
          }
          else if (initialPayloadLen == 127)
          {
               if (frameData.size() < index + 8)
               {
                    ok = false;
                    Logger::instance().error("扩展长度不足 8 字节");
                    return;
               }
               payloadLength = 0;
               for (int i = 0; i < 8; ++i)
               {
                    payloadLength = (payloadLength << 8) | static_cast<uint8_t>(frameData[index + i]);
               }
               index += 8;
          }
          else
          {
               payloadLength = initialPayloadLen;
          }

          // 处理 Masking Key
          if (masked)
          {
               if (frameData.size() < index + 4)
               {
                    ok = false;
                    Logger::instance().error("Masking Key 数据不足");
                    return;
               }
               // frameData 中取 [ frameData.begin() + index，frameData.begin() + index + 4 )
               maskingKey.assign(frameData.begin() + index, frameData.begin() + index + 4);
               index += 4;
          }

          // 提取 Payload 数据
          if (frameData.size() < index + payloadLength)
          {

               ok = false;
               Logger::instance().error("Payload 数据不足");
               return;
          }
          payloadData.assign(frameData.begin() + index, frameData.begin() + index + payloadLength);

          // 解码 Payload 数据（如果有 Masking Key）
          if (masked)
          {
               for (size_t i = 0; i < payloadData.size(); ++i)
               {
                    payloadData[i] ^= maskingKey[i % 4];
               }
          }
     }
};
class WebSocketServer
{
     // SHA-1
     static std::vector<uint8_t> sha1(const std::string &message);
     // Base64编码
     static std::string base64Encode(const std::vector<uint8_t> &input);

     static void readCb(TcpConnection *tc); // 处理接受到的TCP消息

     static std::function<void(TcpConnection *)> onOpen;                            // 连接打开
     static std::function<void(InetAddress addr)> onClose;                          // 关闭时
     static std::function<void(std::string, TcpConnection *tc)> onMessage, onError; // 收到消息时/发生错误时
     TcpServer tcpServer;
     bool isRunning;

     static const size_t MAX_CONNECTED = 100;           // 最大连接数
     static std::map<int, TcpConnection *> tcpConnects; // 管理已经连接的玩家
     static void addconnect(TcpConnection *tc);         // 增加一个新连接
     static void subconnect(TcpConnection *tc);         // 断开一个新连接

     static ThreadPool threadPool;                      // 用于发送消息，或者处理心跳机制

public:
     static void broadcast(const std::string &message); // 群发消息
     static bool send(const std::string &message, TcpConnection *tc);
     WebSocketServer(const InetAddress &addr);
     static void setOnOpen(std::function<void(TcpConnection *)> cb);
     static void setOnclose(std::function<void(InetAddress addr)> cb);
     static void setOnMessage(std::function<void(std::string, TcpConnection *tc)> cb);
     static void setOnError(std::function<void(std::string, TcpConnection *tc)> cb);
     void run();
};
