#include "Tank/Player.h"

Player::Player(int health, float size, b2BodyId bodyId, std::mutex &worldMutex)
    : GameObject(health, bodyId, worldMutex), gold(0), score(0), size(size)
{
}

int Player::getGold()
{
     return gold;
}

int Player::getScore()
{
     return score;
}

void Player::setGold(int gold)
{
     this->gold = gold;
}

void Player::setScore(int score)
{
     this->score = score;
}

void Player::keyDown(std::string key)
{
     if (key == "w")
          keyStates[0] = 1;
     else if (key == "a")
          keyStates[1] = 1;
     else if (key == "s")
          keyStates[2] = 1;
     else if (key == "d")
          keyStates[3] = 1;
}

void Player::keyUp(std::string key)
{
     if (key == "w")
          keyStates[0] = 0;
     else if (key == "a")
          keyStates[1] = 0;
     else if (key == "s")
          keyStates[2] = 0;
     else if (key == "d")
          keyStates[3] = 0;
}

void Player::fixedUpdate()
{
     GameObject::fixedUpdate();
     int moveX = keyStates[3] - keyStates[1];
     int moveY = keyStates[0] - keyStates[2];

     if (moveX == 0 && moveY == 0)
     {
          return;
     }

     b2Vec2 moveDirection = (b2Vec2){0.f, 0.f};
     if (moveX == 0)
     {
          moveDirection.y = moveY;
     }
     else if (moveY == 0)
     {
          moveDirection.x = moveX;
     }
     else
     {
          moveDirection.x = moveX / sqrt(2.f);
          moveDirection.y = moveY / sqrt(2.f);
     }

     b2Vec2 force;
     force.x = moveDirection.x * moveForce;
     force.y = moveDirection.y * moveForce;

     b2Body_ApplyForceToCenter(bodyId, force, true);
}

std::string Player::packData() const
{
     std::ostringstream oss;
     b2Vec2 pos = getPosition();
     oss << "0 " << pos.x << " " << pos.y << " " << getAngle() << " " << size;
     return oss.str();
}
