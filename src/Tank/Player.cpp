#include "Tank/Player.h"

Player::Player(int maxHealth, float size, b2BodyId bodyId, std::mutex &worldMutex)
    : GameObject(maxHealth, bodyId, worldMutex), gold(0), score(0), size(size)
{
     b2Vec2 pos = this->getPosition();
     camera = new Camera(pos.x, pos.y, 5.1, 2.9, this);
     // 初始时自带一个普通炮管
     barrels.emplace_back(new Barrel(0.04, 0.1));
}

Player::~Player()
{
     delete camera;
     for (auto &v : barrels)
          delete v;
}

int Player::getGold()
{
     return gold;
}

int Player::getScore()
{
     return score;
}

void Player::setGold(int gold)
{
     this->gold = gold;
}

void Player::setScore(int score)
{
     this->score = score;
}

void Player::keyDown(std::string key)
{
     if (key == "w")
          keyStates[0] = 1;
     else if (key == "a")
          keyStates[1] = 1;
     else if (key == "s")
          keyStates[2] = 1;
     else if (key == "d")
          keyStates[3] = 1;
}

void Player::keyUp(std::string key)
{
     if (key == "w")
          keyStates[0] = 0;
     else if (key == "a")
          keyStates[1] = 0;
     else if (key == "s")
          keyStates[2] = 0;
     else if (key == "d")
          keyStates[3] = 0;
}

void Player::aim(float angle)
{
     for (auto &v : barrels)
     {
          v->setAngle(angle);
     }
}

void Player::fixedUpdate()
{
     GameObject::fixedUpdate();
     // 重置炮管的打包状态
     for (auto &v : barrels)
     {
          v->resetPackedStatus();
     }

     int moveX = keyStates[3] - keyStates[1];
     int moveY = keyStates[0] - keyStates[2];

     if (moveX == 0 && moveY == 0)
     {
          return;
     }

     b2Vec2 moveDirection = (b2Vec2){0.f, 0.f};
     if (moveX == 0)
     {
          moveDirection.y = moveY;
     }
     else if (moveY == 0)
     {
          moveDirection.x = moveX;
     }
     else
     {
          moveDirection.x = moveX / sqrt(2.f);
          moveDirection.y = moveY / sqrt(2.f);
     }

     b2Vec2 force;
     force.x = moveDirection.x * moveForce;
     force.y = moveDirection.y * moveForce;

     b2Body_ApplyForceToCenter(bodyId, force, true);
}

std::string Player::packData()
{
     if (isPacked)
     {
          return std::string(dataBuf, dataBufLen);
     }
     dataBufLen = 0;
     // 标识符
     uint8_t mark = 2;
     std::memcpy(dataBuf + dataBufLen, &mark, 1);
     dataBufLen += 1;
     // ID
     uint64_t ID = b2StoreBodyId(bodyId);
     std::memcpy(dataBuf + dataBufLen, &ID, 8);
     dataBufLen += 8;
     // 血量
     std::memcpy(dataBuf + dataBufLen, &health, 4);
     dataBufLen += 4;
     // 最大血量
     std::memcpy(dataBuf + dataBufLen, &maxHealth, 4);
     dataBufLen += 4;
     // 金币
     std::memcpy(dataBuf + dataBufLen, &gold, 4);
     dataBufLen += 4;
     // 得分
     std::memcpy(dataBuf + dataBufLen, &score, 4);
     dataBufLen += 4;

     // 几何体
     // 类型
     dataBuf[dataBufLen++] = 0x00; // 圆形

     b2Vec2 pos = getPosition();
     float angle = getAngle();
     // X坐标
     std::memcpy(dataBuf + dataBufLen, &pos.x, 4);
     dataBufLen += 4;
     // Y坐标
     std::memcpy(dataBuf + dataBufLen, &pos.y, 4);
     dataBufLen += 4;
     // 旋转角度
     std::memcpy(dataBuf + dataBufLen, &angle, 4);
     dataBufLen += 4;
     // 属性
     // 半径
     std::memcpy(dataBuf + dataBufLen, &size, 4);
     dataBufLen += 4;

     // 炮管
     for (auto &barrel : barrels)
     {
          std::string tmpstr = barrel->packData();
          std::memcpy(dataBuf + dataBufLen, tmpstr.c_str(), tmpstr.size());
          dataBufLen += tmpstr.size();
     }

     isPacked = true;
     return std::string(dataBuf, dataBufLen);
}

std::string Player::getFrameData()
{
     camera->setPosition(this->getPosition());
     return camera->getFrameData();
}