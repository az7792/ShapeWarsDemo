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

enum class ObjectType : uint8_t
{
     Circle = 0,    // 0 : 圆形
     Triangle = 1,  // 1 : 正三角形
     Square = 2,    // 2 : 正方形
     Rectangle = 3, // 3 : 长方形
     Pentagon = 4   // 4 : 正五边形
};

enum class OperationType : uint8_t
{
     MouseMove = 0,      // 0 : 鼠标移动
     MouseDown = 1,      // 1 : 鼠标按下
     KeyDown = 2,        // 2 : 键盘按下
     KeyUp = 3,          // 3 : 键盘抬起
     CreateCharacter = 4 // 4 : 新建初始角色
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

public:
     GameLoop();
     void loop();
     ~GameLoop() = default;
};