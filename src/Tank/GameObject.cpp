#include "Tank/GameObject.h"

GameObject::GameObject(int maxHealth, b2BodyId bodyId, std::mutex &worldMutex)
    : health(maxHealth), maxHealth(maxHealth), bodyId(bodyId), worldMutex(worldMutex) {}

GameObject::~GameObject()
{
     std::lock_guard<std::mutex> lock(worldMutex);
     if (b2Body_IsValid(bodyId))
          b2DestroyBody(bodyId);
}

void GameObject::fixedUpdate()
{
     b2Vec2 velocity = getVelocity();
     if (velocity.x * velocity.x + velocity.y * velocity.y > maxVelocity * maxVelocity)
     {
          float scale = maxVelocity / sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
          velocity.x *= scale;
          velocity.y *= scale;
          setVelocity(velocity);
     }
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
     std::lock_guard<std::mutex> lock(worldMutex);
     b2Body_SetLinearVelocity(bodyId, velocity);
}

void GameObject::setMaxVelocity(float velocity)
{
     if (velocity >= 0)
          maxVelocity = velocity;
}
