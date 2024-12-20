#pragma once
#include "Player.h"
class Player;
class GameObject;
class Bullet : public GameObject
{
private:
     // 剩余可进行伤害判定的次数，<=0时销毁当前子弹
     int takeDamageNum;

     // 最大判断次数
     int maxTakeDamageNum;

     // 子弹大小(米)
     float size;

     // 当前子弹所属玩家
     Player *owner = nullptr;

     // 当前子弹状态
     bool isAvailable;

public:
     Bullet(int maxHealth, b2BodyId bodyId, World *world);
     ~Bullet();
     MyCategories getType() override { return MyCategories::BULLET; }

     void initOwner(Player *p);

     void setTakeDamageNum(int v);

     // 在每一帧减少一滴血量，用于控制最大射程
     void fixedUpdate() override;

     std::string packData() override;

     void takeDamage(GameObject *obj) override;

     bool getIsDead();

     bool getIsAvailable();

     void setSize(float v);

     void setMaxTakeDamageNum(int v);

     void setIsAvailable(bool v);

     // 销毁当前刚体
     void destroyBody();

     // 初始化
     void onStart(b2BodyId bodyId, b2Vec2 speed);
};
