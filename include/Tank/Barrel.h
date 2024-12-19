#pragma once
#include <string>
#include <cstring>
#include <vector>
#include "Bullet.h"
#include "Player.h"
class Player;
class Bullet;
class Barrel
{
private:
     float width;
     float length;
     Player *owner;
     // 角度(弧度)
     float angle;
     // 角度偏移量(aimAngle + offsetAngle才是实际显示的角度)
     float offsetAngle;

     char dataBuf[4096];    // 数据包
     bool isPacked = false; // 是否已经打包好数据
     int dataBufLen = 0;    // 包的长度

     std::vector<Bullet *> bullets;

     bool canFire;                                       // 是否可以发射
     std::chrono::steady_clock::time_point lastFireTime; // 上次发射的时间
     const std::chrono::milliseconds fireCooldown;       // 发射冷却时间(ms)

     // 子弹的物理定义
     b2BodyDef bodyDef;
     b2ShapeDef shapeDef;
     b2Circle circle;

public:
     Barrel(float width, float length, Player *player);
     virtual ~Barrel();

     // 更新所有子弹
     void fixedUpdate();

     // 发射(修改canFire=true)
     void fire();

     // 获取宽度
     float getWidth() const;

     // 设置宽度
     void setWidth(float newWidth);

     // 获取高度
     float getLength() const;

     // 设置高度
     void setLength(float newLength);

     // 获取角度
     float getAngle() const;

     // 设置角度
     void setAngle(float newAngle);

     // 获取偏移角度
     float getoffsetAngle() const;

     // 设置偏移角度
     void setoffsetAngle(float newOffsetAngle);

     // 获取实际指向的角度
     float getAimAngle() const;

     // 获取炮管打包数据
     std::string packData();

     // 重置打包状态
     void resetPackedStatus();
};
