#include "Tank/GameLoop.h"

void GameLoop::handleOnOpen(const TcpConnection *tc)
{
}

void GameLoop::handleOnClose(const TcpConnection *tc)
{
}

void GameLoop::handleOnMessage(TcpConnection *tc, std::string &&msg)
{
     if (msg.empty())
          return;
     MessageType messageType = static_cast<MessageType>(msg[0]);
     if (messageType == MessageType::PlayerInteractionInfo)
     {
          OperationType operationType = static_cast<OperationType>(msg[1]);
          if (operationType == OperationType::PlayerOperation)
          {
               std::shared_lock<std::shared_mutex> lock(playersSharedMutex); // 读
               if (players.find(tc) != players.end() && players[tc] != nullptr)
               {
                    players[tc]->operationStatus.update(msg.substr(2));
               }
          }
          else if (operationType == OperationType::CreateCharacter)
          {
               std::unique_lock<std::shared_mutex> lock(playersSharedMutex); // 写
               players[tc] = world.addPlayer();
               // 返回创建的角色ID
               std::string strData;
               strData.push_back((uint8_t)0x01);
               strData.push_back((uint8_t)0x03);
               uint64_t ID = b2StoreBodyId(players[tc]->getBodyId());
               char tmpCh[8];
               std::memcpy(tmpCh, &ID, 8);
               strData += std::string(tmpCh, 8);
               webSocketServer.send(strData, tc);
               Logger::instance().info("创建角色成功");
               std::cout << "palyer num: " << players.size() << std::endl;
          }
          else
          {
          }
     }
     else if (messageType == MessageType::Ping)
     {
          webSocketServer.send(msg, tc);
     }
}

GameLoop::GameLoop() : world(5, 5, (b2Vec2){0.f, 0.f}),
                       webSocketServer(InetAddress("0.0.0.0", 7792)),
                       lastTime(std::chrono::steady_clock::now()),
                       frameTime(static_cast<long long>(world.getTimeStep() * 1000))
{
     webSocketServer.setOnOpen(std::bind(&GameLoop::handleOnOpen, this, std::placeholders::_1));
     webSocketServer.setOnMessage(std::bind(&GameLoop::handleOnMessage, this, std::placeholders::_1, std::placeholders::_2));
     webSocketServer.setOnClose(std::bind(&GameLoop::handleOnClose, this, std::placeholders::_1));
}

void GameLoop::loop()
{
     std::thread WebSocketServerThread(&WebSocketServer::run, std::ref(webSocketServer));
     while (true)
     {
          {
               std::shared_lock<std::shared_mutex> lock(playersSharedMutex); // 读
               for (auto &v : players)
                    if (v.second != nullptr)
                         v.second->fixedUpdate();
          }

          world.updateWorld();

          // 处理接触事件
          b2ContactEvents contactEvents = b2World_GetContactEvents(world.getWorldId());
          for (int i = 0; i < contactEvents.beginCount; ++i)
          {
               b2ContactBeginTouchEvent *beginEvent = contactEvents.beginEvents + i;
               GameObject *objA = (GameObject *)b2Body_GetUserData(b2Shape_GetBody(beginEvent->shapeIdA));
               GameObject *objB = (GameObject *)b2Body_GetUserData(b2Shape_GetBody(beginEvent->shapeIdB));
               objA->addDamageTarget(objB);
               objB->addDamageTarget(objA);
               // std::cout << objA->getBodyId().index1 << " " << objB->getBodyId().index1 << std::endl;
          }

          // 处理离开事件
          for (int i = 0; i < contactEvents.endCount; ++i)
          {
               b2ContactEndTouchEvent *endEvent = contactEvents.endEvents + i;

               // 使用 b2Shape_IsValid，因为形状可能已被销毁
               if (b2Shape_IsValid(endEvent->shapeIdA) && b2Shape_IsValid(endEvent->shapeIdB))
               {
                    GameObject *objA = (GameObject *)b2Body_GetUserData(b2Shape_GetBody(endEvent->shapeIdA));
                    GameObject *objB = (GameObject *)b2Body_GetUserData(b2Shape_GetBody(endEvent->shapeIdB));
                    objA->removeDamageTarget(objB);
                    objB->removeDamageTarget(objA);
               }
          }

          // 移除死亡的物体
          // *可能出现玩家A对B的判定时B没死，但其他玩家对B判定导致B死了，而此时A不知道
          // *可能出现玩家A对B判定导致B死亡，而没清除刚体导致接触事件触发，
          //  随后刚体又被清除掉，导致某些物体对nullptr对象进行攻击判定
          // 因此需要单独做清理
          {
               std::shared_lock<std::shared_mutex> lock(playersSharedMutex); // 读
               for (auto &v : players)
                    if (v.second != nullptr)
                         v.second->removeDeadDamageTarget();
          }

          // 休眠到目标时间点
          auto nextFrameTime = lastTime + frameTime;
          std::this_thread::sleep_until(nextFrameTime);
          lastTime = nextFrameTime;

          {
               std::unique_lock<std::shared_mutex> lock(playersSharedMutex); // 写
               for (auto it = players.begin(); it != players.end();)
               {
                    if ((*it).second == nullptr) // 角色已经死亡但每断开连接
                    {
                         ++it;
                         continue;
                    }

                    int num = webSocketServer.send((*it).second->getFrameData(), (*it).first);
                    if (num > 0)
                    {
                         if ((*it).second->getIsDead()) // 发送成功但是玩家死了
                         {
                              world.removeBody((*it).second);
                              (*it).second = nullptr;
                              Logger::instance().info("玩家销毁成功");
                         }
                         ++it;
                    }
                    else // 这儿是连接断开了，不是玩家死亡了
                    {
                         world.removeBody((*it).second);
                         it = players.erase(it);
                         Logger::instance().info("连接销毁成功");
                         std::cout << "palyer num: " << players.size() << std::endl;
                    }
               } // for end
          } // lock end
     } // while end
}