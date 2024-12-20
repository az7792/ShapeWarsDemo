#pragma once
#include "World.h"
#include "WebSocketServer.h"
#include <unordered_map>
#include <shared_mutex>
enum class MessageType : uint8_t
{
     WorldPositionInfo = 1,     // 1: 世界位置原始信息
     PlayerInteractionInfo = 2, // 2: 玩家交互信息
     Ping = 3,                  // 3：Ping,客户端测试延迟用的
};

enum class OperationType : uint8_t
{
     PlayerOperation = 0, // 1 : 玩家操作
     CreateCharacter = 1  // 1 : 新建初始角色
};

class GameLoop
{
private:
     World world;
     WebSocketServer webSocketServer;

     std::shared_mutex playersSharedMutex;
     std::unordered_map<TcpConnection *, Player *> players; // 已经连接的玩家

     void handleOnOpen(TcpConnection *tc);
     void handleOnClose(TcpConnection *tc);
     void handleOnMessage(const std::string msg, TcpConnection *tc);

     std::chrono::steady_clock::time_point lastTime; // 上次刷新的时间
     const std::chrono::milliseconds frameTime;      // 帧间隔
public:
     GameLoop();
     void loop();
     ~GameLoop() = default;
};