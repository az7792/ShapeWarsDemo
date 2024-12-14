#pragma once
#include "box2d/box2d.h"
#include <mutex>
#include <string>
#include <cstring>
#include <vector>
#include <algorithm>
enum class MyCategories // uint32
{
     PLAYER = 0x00000001,
     BULLET = 0x00000002,
     RESOURCE_BLOCK = 0x00000004,
     BORDER_WALL = 0x00000008,
     CAMERA = 0x00000010,
};
class GameObject
{
protected:
     int health;     // 血量
     int maxHealth;  // 最大血量
     int damage = 0; // 解除伤害(每帧的伤害)
     int defend = 0; // 防御力(直接抵消伤害)
     std::vector<GameObject *> damageTargets;

     b2BodyId bodyId;
     // 创建body需要对世界进行写操作，因此需要锁
     std::mutex &worldMutex;

     char dataBuf[4096];       // 数据包
     bool isPacked = false;    // 是否已经打包好数据
     int dataBufLen = 0;       // 包的长度
     float maxVelocity = 0.5f; // 最大线速度 m/s *并不是玩家控制的移动速度

     int32_t groupIndex = 0; // 物体所在碰撞组(默认不在任何组)

     bool isVisible = true; // 物体是可见(是否透明)

public:
     GameObject(int maxHealth, b2BodyId bodyId, std::mutex &worldMutex);

     // 删除前请确保世界已经停止模拟
     virtual ~GameObject();

     /**
      * @brief 对目标造成伤害
      * @param obj 被攻击的对象
      */
     virtual void takeDamage(GameObject *obj);

     // 将攻击对象加入列表
     virtual void addDamageTarget(GameObject *obj);
     // 将攻击对象移除类别
     virtual void removeDamageTarget(GameObject *obj);

     // 打包数据用于传输到前端进行渲染
     virtual std::string packData() { return ""; };

     // virtual void update() = 0;

     // 固定时间间隔更新一次(默认实现会限制线速度<=maxVelocity)
     virtual void fixedUpdate();

     float getX() const;
     float getY() const;
     float getMaxVelocity() const;
     b2Vec2 getPosition() const;
     int getHealth() const;
     int getMaxHealth() const;
     b2BodyId getBodyId() const;
     // 获取角度(弧度制)
     float getAngle() const;
     // 获取线速度
     b2Vec2 getVelocity();

     // 获取伤害
     int getDamage() const;

     // 获取物体是否死亡
     bool getIsDead() const;

     bool getIsVisible() const;

     int getDefend() const;

     void setHealth(int value);
     void setMaxHealth(int value);
     // 设置线速度
     void setVelocity(b2Vec2 velocity);
     // 设置最大线速度
     void setMaxVelocity(float velocity);
     // 初始化groupIndex
     void initGroupIndex(int32_t value);

     void setIsVisible(bool v);

     void setDamage(int v);

     void setDefend(int v);
};