#include "Tank/Barrel.h"

Barrel::Barrel()
{
}

Barrel::Barrel(float width, float length) : width(width), length(length), angle(0), offsetAngle(0)
{
}

Barrel::~Barrel()
{
}

float Barrel::getWidth() const
{
     return width;
}

void Barrel::setWidth(float newWidth)
{
     width = newWidth;
}

float Barrel::getLength() const
{
     return length;
}

void Barrel::setLength(float newLength)
{
     length = newLength;
}

float Barrel::getAngle() const
{
     return angle;
}

void Barrel::setAngle(float newAngle)
{
     angle = newAngle;
}

float Barrel::getoffsetAngle() const
{
     return offsetAngle;
}

void Barrel::setoffsetAngle(float newOffsetAngle)
{
     offsetAngle = newOffsetAngle;
}

float Barrel::getAimAngle() const
{
     return angle + offsetAngle;
}

std::string Barrel::packData()
{
     if (isPacked)
     {
          return std::string(dataBuf, dataBufLen);
     }
     dataBufLen = 0;
     // 宽度
     std::memcpy(dataBuf + dataBufLen, &width, 4);
     dataBufLen += 4;
     // 高度
     std::memcpy(dataBuf + dataBufLen, &length, 4);
     dataBufLen += 4;
     // 实际角度
     float tmpAngle = getAimAngle();
     std::memcpy(dataBuf + dataBufLen, &tmpAngle, 4);
     dataBufLen += 4;
     isPacked = true;

     return std::string(dataBuf, dataBufLen);
}
void Barrel::resetPackedStatus()
{
     isPacked = false;
}