#include "Tank/GameObject.h"

GameObject::GameObject(int maxHealth, b2BodyId bodyId, World *world)
    : health(maxHealth), maxHealth(maxHealth), bodyId(bodyId), world(world) {}

GameObject::~GameObject()
{
     std::lock_guard<std::mutex> lock(world->getWorldMutex());
     if (b2Body_IsValid(bodyId))
          b2DestroyBody(bodyId);
}

void GameObject::takeDamage(GameObject *obj)
{
     if (obj->getDefend() >= damage) // 完全减伤
          return;
     int H = obj->getHealth();
     H -= damage - obj->getDefend();
     obj->setHealth(H);
}

void GameObject::addDamageTarget(GameObject *obj)
{
     damageTargets.push_back(obj);
}

void GameObject::removeDamageTarget(GameObject *obj)
{
     auto it = std::find(damageTargets.begin(), damageTargets.end(), obj); // O(n)，n一般来说很小(<10)
     if (it != damageTargets.end())
     {
          std::swap(*it, damageTargets.back());
          damageTargets.pop_back();
     }
}

void GameObject::fixedUpdate()
{
     // 伤害判定
     for (int i = 0; i < static_cast<int>(damageTargets.size());)
     {
          takeDamage(damageTargets[i]);
          if (damageTargets[i]->getIsDead())
          {
               std::swap(damageTargets[i], damageTargets.back());
               damageTargets.pop_back();
          }
          else
               ++i;
     }

     // 控制线速度
     b2Vec2 velocity = getVelocity();
     if (velocity.x * velocity.x + velocity.y * velocity.y > maxVelocity * maxVelocity)
     {
          float scale = maxVelocity / sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
          velocity.x *= scale;
          velocity.y *= scale;
          setVelocity(velocity);
     }

     // 重置打包状态
     isPacked = false;
}

float GameObject::getX() const
{
     return b2Body_GetPosition(bodyId).x;
}

float GameObject::getY() const
{
     return b2Body_GetPosition(bodyId).y;
}

float GameObject::getMaxVelocity() const
{
     return maxVelocity;
}
b2Vec2 GameObject::getPosition() const
{
     return b2Body_GetPosition(bodyId);
}

float GameObject::getAngle() const
{
     return b2Rot_GetAngle(b2Body_GetRotation(bodyId));
}

int GameObject::getHealth() const
{
     return health;
}

int GameObject::getMaxHealth() const
{
     return maxHealth;
}

b2Vec2 GameObject::getVelocity()
{
     return b2Body_GetLinearVelocity(bodyId);
}

bool GameObject::getIsVisible() const
{
     return isVisible;
}

int GameObject::getDefend() const
{
     return defend;
}

int GameObject::getDamage() const
{
     return damage;
}

int32_t GameObject::getGroupIndex()
{
     return groupIndex;
}

World *GameObject::getWorld()
{
     return world;
}

b2BodyId GameObject::getBodyId() const
{
     return bodyId;
}

void GameObject::setHealth(int value)
{
     health = value;
}

void GameObject::setMaxHealth(int value)
{
     maxHealth = value;
}

void GameObject::setVelocity(b2Vec2 velocity)
{
     std::lock_guard<std::mutex> lock(world->getWorldMutex());
     b2Body_SetLinearVelocity(bodyId, velocity);
}

void GameObject::setMaxVelocity(float velocity)
{
     if (velocity >= 0)
          maxVelocity = velocity;
}

void GameObject::initGroupIndex(int32_t value)
{
     groupIndex = value;
}

void GameObject::setIsVisible(bool v)
{
     isVisible = v;
}

void GameObject::setDamage(int v)
{
     damage = v;
}

void GameObject::setDefend(int v)
{
     defend = v;
}

void GameObject::setBodyId(b2BodyId bodyId)
{
     this->bodyId = bodyId;
}

bool GameObject::getIsDead() const
{
     return health <= 0;
}
