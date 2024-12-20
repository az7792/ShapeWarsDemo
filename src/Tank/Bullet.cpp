#include "Tank/Bullet.h"
Bullet::Bullet(int maxHealth, b2BodyId bodyId, World *world)
    : GameObject(maxHealth, bodyId, world), isAvailable(true)
{
}

Bullet::~Bullet()
{
}

void Bullet::initOwner(Player *p)
{
     owner = p;
}

void Bullet::setTakeDamageNum(int v)
{
     takeDamageNum = v;
}

void Bullet::fixedUpdate()
{
     GameObject::fixedUpdate();
     health--; // 限制飞行距离
     if (getIsDead())
     {
          destroyBody();
     }
}

std::string Bullet::packData()
{
     if (isPacked)
     {
          return std::string(dataBuf, dataBufLen);
     }
     dataBufLen = 0;
     // 标识符
     uint8_t mark = 4;
     std::memcpy(dataBuf + dataBufLen, &mark, 1);
     dataBufLen += 1;

     // 所属碰撞组
     std::memcpy(dataBuf + dataBufLen, &groupIndex, 4);
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

     isPacked = true;
     return std::string(dataBuf, dataBufLen);
}

void Bullet::takeDamage(GameObject *obj)
{
     GameObject::takeDamage(obj);
     takeDamageNum--;

     // 判断是否是否死亡并根据类型觉得是否结算金币和积分
     if (obj->getIsDead() && (obj->getType() == MyCategories::PLAYER || obj->getType() == MyCategories::RESOURCE_BLOCK))
     {
          // 结算奖励
          owner->addGold(obj->getGold() / 2);
          owner->addScore(obj->getScore() / 2);
     }
}

bool Bullet::getIsDead()
{
     return health <= 0 || takeDamageNum <= 0;
}

bool Bullet::getIsAvailable()
{
     return isAvailable;
}

void Bullet::setSize(float v)
{
     size = v;
}

void Bullet::setMaxTakeDamageNum(int v)
{
     maxTakeDamageNum = v;
}

void Bullet::setIsAvailable(bool v)
{
     isAvailable = v;
}

void Bullet::destroyBody()
{
     // std::lock_guard<std::mutex> lock(owner->getWorld()->getWorldMutex());
     if (b2Body_IsValid(bodyId))
     {
          b2DestroyBody(bodyId);
          isAvailable = false;
          damageTargets.clear();
     }
}

void Bullet::onStart(b2BodyId bodyId, b2Vec2 speed)
{
     this->bodyId = bodyId;
     health = maxHealth;
     takeDamageNum = maxTakeDamageNum;
     isAvailable = true;
     damageTargets.clear();
     b2Body_SetLinearVelocity(bodyId, speed);
     b2Body_SetUserData(bodyId, (void *)this);
}
