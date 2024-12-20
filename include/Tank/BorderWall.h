#pragma once
#include "GameObject.h"
class World;
class BorderWall : public GameObject
{
public:
     BorderWall(int maxHealth, b2BodyId bodyId, World *world) : GameObject(maxHealth, bodyId, world) {};
     ~BorderWall() {};
     MyCategories getType() override { return MyCategories::BORDER_WALL; }

     void takeDamage(GameObject *obj) override {};
     void addDamageTarget(GameObject *obj) override {};
     void removeDamageTarget(GameObject *obj) override {};
     void fixedUpdate() override {};
};