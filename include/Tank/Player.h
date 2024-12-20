#pragma once
#include "GameObject.h"
#include "Camera.h"
#include "Barrel.h"
#include "OperationStatus.h"
#include <vector>
#include <cmath>
class GameObject;
class Barrel;
class Camera;
class World;
class Player : public GameObject
{
private:
     int gold;               // 金币
     int score;              // 积分
     float size;             // 大小(半径单位米)
     float moveForce = 0.1f; // 单位 N
     Camera *camera = nullptr;

     std::vector<Barrel *> barrels;

public:
     OperationStatus operationStatus;
     Player(int maxHealth, float size, b2BodyId bodyId, World *world);
     ~Player();
     MyCategories getType() override { return MyCategories::PLAYER; }

     // 清理剩余死亡物体，清理攻击列表
     void removeDeadDamageTarget() override;

     int getGold() override;
     int getScore() override;

     void addGold(int v);
     void subGold(int v);
     void addScore(int v);
     void subScore(int v);

     void setGold(int gold);
     void setScore(int score);

     void fixedUpdate() override;

     void takeDamage(GameObject *obj) override;

     std::string packData() override;

     std::string getFrameData();
};