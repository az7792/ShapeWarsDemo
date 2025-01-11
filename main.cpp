#include <iostream>
#include <unistd.h>
#include "box2d/box2d.h"
#include "Tank/GameLoop.h"
using namespace std;
int main()
{
     Logger::instance().setLevel(LogLevel::DEBUG);
     GameLoop gameLoop;

     gameLoop.loop();
     return 0;
     // // 初始化
     // //  定义
      b2WorldDef worldDef = b2DefaultWorldDef();
     // worldDef.gravity = (b2Vec2){0, 0}; // 采用欧几里得坐标系，(0，-10)重力的方向(x,y)
     // // 创建（创建采用复制，因此定义可以在被创建后销毁，例如在栈上创建然后离开作用域后销毁）
     // b2WorldId worldId = b2CreateWorld(&worldDef);

     // // 创建一个物体(刚体,物理对象的主体)，默认为静态的，一个 Body 可以包含一个或多个 Shape，从而构建复杂的物体（例如由多个矩形组成的卡车）
     // b2BodyDef bodyDef = b2DefaultBodyDef();
     // bodyDef.position = (b2Vec2){300.f, -300.f}; // 位置(x,y) = （0，-12）;
     // b2BodyId bodyId = b2CreateBody(worldId, &bodyDef);

     // // 创建一个多边形(表示物体的轮廓,定义刚体的外形)
     // b2Polygon groundBox = b2MakeBox(50.0f, 2.0f); // b2MakeBox(半宽(x),半高(y))，因此实际大小为100*4

     // // 创建形状(定义刚体的几何形状，决定其物理行为，碰撞特性)
     // b2ShapeDef shapeDef = b2DefaultShapeDef();
     // shapeDef.enableContactEvents = true;
     // b2CreatePolygonShape(bodyId, &shapeDef, &groundBox); // 为指定的刚体（Body）创建一个多边形形状，并将该形状附加到刚体上

     // bodyDef = b2DefaultBodyDef();
     // bodyDef.type = b2_dynamicBody; // 设置为动态物体
     // bodyDef.position = (b2Vec2){0, 10};
     // b2BodyId dynamicBodyId = b2CreateBody(worldId, &bodyDef);

     // b2Polygon dynamicBox = b2MakeBox(5, 5);

     // shapeDef = b2DefaultShapeDef();
     // shapeDef.density = 1.f;   // 默认为1
     // shapeDef.friction = 0.1f; // 动态物体需要设置密度和摩擦系数

     // b2CreatePolygonShape(dynamicBodyId, &shapeDef, &dynamicBox);

     // b2Segment s1, s2, s3, s4;
     // bodyDef = b2DefaultBodyDef();
     // bodyDef.position = (b2Vec2){0.f, 0.f};
     // b2BodyId Wall = b2CreateBody(worldId, &bodyDef);
     // shapeDef = b2DefaultShapeDef();
     // s1.point1 = (b2Vec2){-300.f, 300.f};
     // s1.point2 = (b2Vec2){300.f, 300.f};
     // b2CreateSegmentShape(Wall, &shapeDef, &s1);

     // s2.point1 = (b2Vec2){300.f, 300.f};
     // s2.point2 = (b2Vec2){300.f, -300.f};
     // b2CreateSegmentShape(Wall, &shapeDef, &s2);

     // s3.point1 = (b2Vec2){300.f, -300.f};
     // s3.point2 = (b2Vec2){-300.f, -300.f};
     // b2CreateSegmentShape(Wall, &shapeDef, &s3);

     // s4.point1 = (b2Vec2){-300.f, -300.f};
     // s4.point2 = (b2Vec2){-300.f, 300.f};
     // b2CreateSegmentShape(Wall, &shapeDef, &s4);

     // // 仿真
     // float timeStep = 1.0f / 20.f; // 每次循环的间隔，最好不要把他和帧率绑定
     // int subStepCount = 4;         // 子步长，将一段仿真时间划分为更小的时间，推荐4，太少会提高性能，但会牺牲精度，太多反之
     // // 例如，在 60Hz(1/60s) 的 timeStep 和 4 个子步长下，子步长的操作频率为 240Hz（1/(60*4)）。如果使用 8 个子步长，则为 480Hz（1/(60*8)）。

     // this_thread::sleep_for(std::chrono::seconds(5));
     // while (1)
     // {
     //      /*
     //      0 圆
     //      1 正三角形
     //      2 正方形
     //      3 长方形
     //      4 正五边形
     //      x y r 属性
     //      */
     //      b2World_Step(worldId, timeStep, subStepCount);
     //      b2ContactEvents contactEvents = b2World_GetContactEvents(worldId);
     //      for (int i = 0; i < contactEvents.beginCount; ++i)
     //      {
     //           b2ContactBeginTouchEvent *beginEvent = contactEvents.beginEvents + i;
     //           cout << beginEvent->shapeIdA.index1 << " " << beginEvent->shapeIdB.index1 << endl;
     //      }
     //      b2Vec2 p = b2Body_GetPosition(dynamicBodyId);
     //      b2Rot r = b2Body_GetRotation(dynamicBodyId);
     //      b2Body_ApplyForceToCenter(dynamicBodyId, F, true);
     //      F.x = F.y = 0;
     //      //cout << std::fixed << p.x << " " << p.y << endl;
     //   std::ostringstream oss;
     //      oss << std::fixed << "2 " << p.x << " " << p.y << " " << b2Rot_GetAngle(r) << " 10 3 ";
     //      oss << std::fixed << b2Body_GetPosition(bodyId).x << " " << b2Body_GetPosition(bodyId).y << " " << b2Rot_GetAngle(b2Body_GetRotation(bodyId)) << " 100 4";
     //      WebSocketServer::broadcast(oss.str());
     //      this_thread::sleep_for(std::chrono::duration<double>(timeStep));
     // }
     // return 0;
}