#include "Tank/Player.h"
Player::Player(int maxHealth, float size, b2BodyId bodyId, World *world)
    : GameObject(maxHealth, bodyId, world), gold(0), score(0), size(size)
{
     b2Vec2 pos = this->getPosition();
     camera = new Camera(pos.x, pos.y, 5.1, 2.9, this);
     // 初始时自带一个普通炮管
     barrels.emplace_back(new Barrel(0.04, 0.1, this));
}

Player::~Player()
{
     delete camera;
     for (auto &v : barrels)
          delete v;
}

void Player::removeDeadDamageTarget()
{
     GameObject::removeDeadDamageTarget();
     // 通知炮管去清理子弹的 判定列表
     for (auto &v : barrels)
          v->removeDeadDamageTarget();
}

int Player::getGold()
{
     return gold;
}

int Player::getScore()
{
     return score;
}

void Player::addGold(int v)
{
     gold += v;
}

void Player::subGold(int v)
{
     gold -= v;
}

void Player::addScore(int v)
{
     score += v;
}

void Player::subScore(int v)
{
     score -= v;
}

void Player::setGold(int gold)
{
     this->gold = gold;
}

void Player::setScore(int score)
{
     this->score = score;
}

void Player::fixedUpdate()
{
     GameObject::fixedUpdate();
     // 重置炮管的打包状态与更新炮管角度
     for (auto &v : barrels)
     {
          v->resetPackedStatus();
          v->setAngle(atan2(operationStatus.y - this->getY(), operationStatus.x - this->getX()));
          if (operationStatus.getKeyStatus(LEFT))
               v->fire();
          v->fixedUpdate();
     }

     int moveX = operationStatus.getKeyStatus(D) - operationStatus.getKeyStatus(A);
     int moveY = operationStatus.getKeyStatus(W) - operationStatus.getKeyStatus(S);

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

void Player::takeDamage(GameObject *obj)
{
     GameObject::takeDamage(obj);
     // 判断是否是否死亡并根据类型觉得是否结算金币和积分
     if (obj->getIsDead() && (obj->getType() == MyCategories::PLAYER || obj->getType() == MyCategories::RESOURCE_BLOCK))
     {
          // 结算奖励
          addGold(obj->getGold() / 2);
          addScore(obj->getScore() / 2);
     }
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
     // 所属碰撞组
     std::memcpy(dataBuf + dataBufLen, &groupIndex, 4);
     dataBufLen += 4;
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
     if (getIsDead())
     {
          std::string str = {2, 2};
          return str;
     }
     camera->setPosition(this->getPosition());
     return camera->getFrameData();
}