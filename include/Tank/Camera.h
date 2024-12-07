#pragma once
#include "box2d/box2d.h"
#include "Player.h"
class Camera
{
     b2Vec2 position;
     float width; // 单位米
     float height;
     b2QueryFilter filter;
     GameObject *owner; // 摄像机所属GameObject
     std::string objectData;

     uint16_t scale = 500;
     uint16_t playerNum;

public:
     // 摄像机中心坐标(x,y)，宽高(2*w,2*h)
     Camera(float x, float y, float w, float h, GameObject *gb);
     ~Camera() = default;

     void setOwner(GameObject *gb);
     void setPosition(b2Vec2 pos);
     void setWidth(float w);
     void setHeight(float h);

     b2Vec2 getPosition();
     GameObject *getOwner();
     static bool MyOverlapResultFcn(b2ShapeId shapeId, void *context);
     std::string &getObjectData();

     std::string getFrameData();
     void playerNumAdd();
};