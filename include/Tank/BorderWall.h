#pragma once
#include "GameObject.h"

class BorderWall : public GameObject
{
public:
     BorderWall(int maxHealth, b2BodyId bodyId, std::mutex &worldMutex) : GameObject(maxHealth, bodyId, worldMutex) {};
     ~BorderWall(){};

     void takeDamage(GameObject *obj) override {};
     void addDamageTarget(GameObject *obj) override {};
     void removeDamageTarget(GameObject *obj) override {};
     void fixedUpdate() override {};
};