#pragma once
#include "GameObject.h"
#include "Camera.h"
#include "Barrel.h"
#include "OperationStatus.h"
#include <vector>
#include <cmath>
class Camera;
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
     Player(int maxHealth, float size, b2BodyId bodyId, std::mutex &worldMutex);
     ~Player();
     int getGold();
     int getScore();
     void setGold(int gold);
     void setScore(int score);

     void fixedUpdate() override;

     std::string packData() override;

     std::string getFrameData();
};