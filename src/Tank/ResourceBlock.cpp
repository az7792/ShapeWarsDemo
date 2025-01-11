#include "Tank/ResourceBlock.h"

ResourceBlock::ResourceBlock(int maxHealth, b2BodyId bodyId, World *world) : GameObject(maxHealth, bodyId, world)
{
}

ResourceBlock::~ResourceBlock()
{
}

int ResourceBlock::getGold()
{
     return gold;
}

int ResourceBlock::getScore()
{
     return score;
}

int ResourceBlock::getSize()
{
     return size;
}

ResourceBlockType ResourceBlock::getResourceBlockType()
{
     return resourceBlockType;
}

void ResourceBlock::setGold(int v)
{
     gold = v;
}

void ResourceBlock::setScore(int v)
{
     score = v;
}

void ResourceBlock::setSize(float v)
{
     size = v;
}

void ResourceBlock::setResourceBlockType(ResourceBlockType v)
{
     resourceBlockType = v;
}

void ResourceBlock::addDamageTarget(GameObject *obj)
{
     MyCategories objType = obj->getType();
     if (objType == MyCategories::RESOURCE_BLOCK || objType == MyCategories::BORDER_WALL)
          return; // 过滤其他资源方块和边界墙
     GameObject::addDamageTarget(obj);
}

void ResourceBlock::removeDamageTarget(GameObject *obj)
{
     MyCategories objType = obj->getType();
     if (objType == MyCategories::RESOURCE_BLOCK || objType == MyCategories::BORDER_WALL)
          return; // 过滤其他资源方块和边界墙
     GameObject::removeDamageTarget(obj);
}

std::string ResourceBlock::packData()
{
     if (isPacked)
     {
          return std::string(dataBuf, dataBufLen);
     }
     dataBufLen = 0;
     // 标识符
     uint8_t mark = 5;
     std::memcpy(dataBuf + dataBufLen, &mark, 1);
     dataBufLen += 1;
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
     dataBuf[dataBufLen++] = static_cast<uint8_t>(resourceBlockType); // 圆形

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
