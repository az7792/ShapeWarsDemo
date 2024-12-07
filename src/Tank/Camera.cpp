#include "Tank/Camera.h"

Camera::Camera(float x, float y, float w, float h, GameObject *gb) : width(w), height(h), owner(gb)
{
     position = (b2Vec2){x, y};

     // 默认的b2QueryFilter
     filter.categoryBits = (uint64_t)MyCategories::CAMERA;
     filter.maskBits = (uint64_t)MyCategories::PLAYER |
                       (uint64_t)MyCategories::RESOURCE_BLOCK |
                       (uint64_t)MyCategories::BULLET;
}

void Camera::setOwner(GameObject *gb)
{
     owner = gb;
}

void Camera::setPosition(b2Vec2 pos)
{
     position = pos;
}

void Camera::setWidth(float w)
{
     width = w;
}

void Camera::setHeight(float h)
{
     height = h;
}

b2Vec2 Camera::getPosition()
{
     return position;
}

GameObject *Camera::getOwner()
{
     return owner;
}

bool Camera::MyOverlapResultFcn(b2ShapeId shapeId, void *context)
{
     GameObject *gameObject = static_cast<GameObject *>(b2Body_GetUserData(b2Shape_GetBody(shapeId)));
     if (gameObject->getIsVisible() == false)
          return true;
     Camera *camera = static_cast<Camera *>(context);
     if (gameObject == camera->getOwner()) // 此部分由owner自己处理
          return true;
     camera->playerNumAdd();
     camera->getObjectData() += gameObject->packData();
     return true;
}

std::string Camera::getFrameData()
{
     objectData.clear();
     char tmpBuf[4];

     // 摄像机
     std::memcpy(tmpBuf, &position.x, 4);
     objectData += std::string(tmpBuf, 4);
     std::memcpy(tmpBuf, &position.y, 4);
     objectData += std::string(tmpBuf, 4);
     std::memcpy(tmpBuf, &scale, 2);
     objectData += std::string(tmpBuf, 2);

     // 玩家
     if (owner->getIsVisible())
     {
          playerNum = 1;
          objectData.push_back((uint8_t)0x01);
          objectData += std::string(tmpBuf, 2);
          objectData += owner->packData();
     }
     else
     {
          objectData.push_back((uint8_t)0x00);
          objectData += std::string(tmpBuf, 2);
          playerNum = 0;
     }

     b2AABB aabb;
     aabb.lowerBound = {position.x - width, position.y - height};
     aabb.upperBound = {position.x + width, position.y + height};

     b2World_OverlapAABB(b2Body_GetWorld(owner->getBodyId()), aabb, filter, MyOverlapResultFcn, this);

     // 填充 playerNum
     std::memcpy(tmpBuf, &playerNum, 2);
     objectData[11] = tmpBuf[0];
     objectData[12] = tmpBuf[1];
     return objectData;
}

void Camera::playerNumAdd()
{
     playerNum++;
}
std::string &Camera::getObjectData()
{
     return objectData;
}
