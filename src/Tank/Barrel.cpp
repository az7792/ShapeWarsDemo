#include "Tank/Barrel.h"

Barrel::Barrel(float width, float length, Player *owner)
    : width(width), length(length), owner(owner), angle(0), offsetAngle(0),
      canFire(false), fireCooldown(200), bodyDef(b2DefaultBodyDef()), shapeDef(b2DefaultShapeDef())
{
     bodyDef.type = b2_dynamicBody;
     bodyDef.position = (b2Vec2){0.f, 0.f};

     shapeDef.density = 10.f;  // 默认为1
     shapeDef.friction = 0.1f; // 动态物体需要设置密度和摩擦系数
     shapeDef.filter.categoryBits = (uint64_t)MyCategories::BULLET;
     shapeDef.filter.maskBits = (uint64_t)MyCategories::BULLET |
                                (uint64_t)MyCategories::RESOURCE_BLOCK |
                                (uint64_t)MyCategories::CAMERA |
                                (uint64_t)MyCategories::PLAYER;
     shapeDef.enableContactEvents = true;

     circle.center = b2Vec2_zero; // 这个坐标是相对bodyDef.position而言的偏移量
     circle.radius = width / 2;
}

Barrel::~Barrel()
{
     for (auto &v : bullets)
     {
          delete v;
     }
     bullets.clear();
}

void Barrel::fixedUpdate() // *这一步是在世界步进前进行的，因此创建物体不需要锁
{
     for (auto &v : bullets)
     {
          if (v->getIsAvailable())
               v->fixedUpdate();
          else if (canFire) // 当前子弹不可用，并且当前帧需要发射子弹
          {
               // 创建一个新子弹
               bodyDef.position = owner->getPosition();

               b2BodyId bodyId = b2CreateBody(owner->getWorld()->getWorldId(), &bodyDef);

               b2CreateCircleShape(bodyId, &shapeDef, &circle);

               float speed = 0.5;
               float ang = getAimAngle();
               v->onStart(bodyId, {speed * cos(ang), speed * sin(ang)});
               canFire = false;
          }
     }
     if (canFire) // 说明没有空闲子弹可用
     {
          // 创建一个新子弹
          bodyDef.position = owner->getPosition();

          // 确保不与玩家自己碰撞
          shapeDef.filter.groupIndex = owner->getGroupIndex();

          b2BodyId bodyId = b2CreateBody(owner->getWorld()->getWorldId(), &bodyDef);

          b2CreateCircleShape(bodyId, &shapeDef, &circle);

          float speed = 0.5;
          float ang = getAimAngle();

          Bullet *tmpBullet = new Bullet(300, bodyId, owner->getWorld());
          tmpBullet->setSize(circle.radius);
          tmpBullet->initOwner(owner);
          tmpBullet->initGroupIndex(owner->getGroupIndex());
          tmpBullet->setMaxTakeDamageNum(3);
          tmpBullet->setDamage(5);

          tmpBullet->onStart(bodyId, {speed * cos(ang), speed * sin(ang)});
          bullets.emplace_back(tmpBullet);

          canFire = false;
     }
}

void Barrel::fire()
{
     auto currentTime = std::chrono::steady_clock::now(); // 获取当前时间

     // 计算当前时间与上次发射时间的间隔
     auto timeSinceLastFire = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastFireTime);

     // 判断间隔是否大于冷却时间
     if (timeSinceLastFire > fireCooldown)
     {
          canFire = true;
          lastFireTime = currentTime;
     }
}

float Barrel::getWidth() const
{
     return width;
}

void Barrel::setWidth(float newWidth)
{
     width = newWidth;
}

float Barrel::getLength() const
{
     return length;
}

void Barrel::setLength(float newLength)
{
     length = newLength;
}

float Barrel::getAngle() const
{
     return angle;
}

void Barrel::setAngle(float newAngle)
{
     angle = newAngle;
}

float Barrel::getoffsetAngle() const
{
     return offsetAngle;
}

void Barrel::setoffsetAngle(float newOffsetAngle)
{
     offsetAngle = newOffsetAngle;
}

float Barrel::getAimAngle() const
{
     return angle + offsetAngle;
}

std::string Barrel::packData()
{
     if (isPacked)
     {
          return std::string(dataBuf, dataBufLen);
     }
     dataBufLen = 0;
     // 宽度
     std::memcpy(dataBuf + dataBufLen, &width, 4);
     dataBufLen += 4;
     // 长度
     std::memcpy(dataBuf + dataBufLen, &length, 4);
     dataBufLen += 4;
     // 实际角度
     float tmpAngle = getAimAngle();
     std::memcpy(dataBuf + dataBufLen, &tmpAngle, 4);
     dataBufLen += 4;
     isPacked = true;

     return std::string(dataBuf, dataBufLen);
}
void Barrel::resetPackedStatus()
{
     isPacked = false;
}