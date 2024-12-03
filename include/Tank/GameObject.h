#pragma once
#include "../box2d/box2d.h"
#include <mutex>
#include <string>
#include <sstream>
class GameObject
{
protected:
     int health; // 血量
     b2BodyId bodyId;
     // 创建body需要对世界进行写操作，因此需要锁
     std::mutex &worldMutex;

     float maxVelocity = 0.5f; // 最大线速度 m/s

public:
     GameObject(int health, b2BodyId bodyId, std::mutex &worldMutex);

     // 删除前请确保世界已经停止模拟
     virtual ~GameObject();

     // virtual void takeDamage(int damage);

     // 打包数据用于传输到前端进行渲染
     virtual std::string packData() const = 0;

     //virtual void update() = 0;

     // 固定时间间隔更新一次(默认实现会限制线速度<=maxVelocity)
     virtual void fixedUpdate();

     float getX() const;
     float getY() const;
     float getMaxVelocity() const;
     b2Vec2 getPosition() const;
     int getHealth() const;
     b2BodyId getBodyId() const;
     // 获取角度(弧度制)
     float getAngle() const;
     // 获取线速度
     b2Vec2 getVelocity();

     void setHealth(int value);
     // 设置线速度
     void setVelocity(b2Vec2 velocity);
     // 设置最大线速度
     void setMaxVelocity(float velocity);
};