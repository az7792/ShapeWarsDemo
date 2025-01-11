#pragma once
#include "GameObject.h"
class ResourceBlock : public GameObject
{
private:
     int gold;  // 金币
     int score; // 积分
     float size;
     ResourceBlockType resourceBlockType;

public:
     ResourceBlock(int maxHealth, b2BodyId bodyId, World *world);
     ~ResourceBlock();
     int getGold();
     int getScore();
     int getSize();
     ResourceBlockType getResourceBlockType();

     void setGold(int v);
     void setScore(int v);
     void setSize(float v);
     void setResourceBlockType(ResourceBlockType v);

     // 将攻击对象加入列表
     void addDamageTarget(GameObject *obj) override;
     // 将攻击对象移除类别
     void removeDamageTarget(GameObject *obj) override;

     // 打包数据包
     std::string packData() override;
};
