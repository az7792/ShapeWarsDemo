#pragma once
#include "GameObject.h"
#include "Camera.h"
#include "Barrel.h"
#include <vector>
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
     bool keyStates[4] = {0, 0, 0, 0}; // w a s d是否按下
     Player(int maxHealth, float size, b2BodyId bodyId, std::mutex &worldMutex);
     ~Player();
     int getGold();
     int getScore();
     void setGold(int gold);
     void setScore(int score);

     // 按键按下
     void keyDown(std::string key);

     // 按键抬起
     void keyUp(std::string key);

     // 鼠标移动(瞄准，炮管跟随鼠标方向)
     void aim(float angle);

     // 鼠标按下(旋转到指定方向发射子弹)
     //void fire(float angle);

     void fixedUpdate() override;

     std::string packData() override;

     std::string getFrameData();
};