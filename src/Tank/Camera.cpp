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
     camera->getObjectData() += gameObject->packData();
     return true;
}

std::string Camera::getFrameData()
{
     objectData.clear();
     char tmpBuf[4];
     // MessageType
     objectData.push_back((uint8_t)0x01);
     
     // 摄像机
     objectData.push_back((uint8_t)0x01);
     std::memcpy(tmpBuf, &position.x, 4);
     objectData += std::string(tmpBuf, 4);
     std::memcpy(tmpBuf, &position.y, 4);
     objectData += std::string(tmpBuf, 4);
     std::memcpy(tmpBuf, &scale, 2);
     objectData += std::string(tmpBuf, 2);

     // 玩家
     b2AABB aabb;
     aabb.lowerBound = {position.x - width, position.y - height};
     aabb.upperBound = {position.x + width, position.y + height};

     b2World_OverlapAABB(b2Body_GetWorld(owner->getBodyId()), aabb, filter, MyOverlapResultFcn, this);
     return objectData;
}

std::string &Camera::getObjectData()
{
     return objectData;
}
