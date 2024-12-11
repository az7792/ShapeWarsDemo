#include "Tank/World.h"

World::World(b2Vec2 gravity = (b2Vec2){0, 0})
{
     b2WorldDef worldDef = b2DefaultWorldDef();
     worldDef.gravity = gravity;
     worldId = b2CreateWorld(&worldDef);
     Logger::instance().info("世界" + std::to_string(worldId.index1) + "创建成功");
}

World::World(float w, float h, b2Vec2 gravity = (b2Vec2){0, 0})
{
     b2WorldDef worldDef = b2DefaultWorldDef();
     worldDef.gravity = gravity;
     worldId = b2CreateWorld(&worldDef);

     // 创建墙
     b2Segment s;
     b2BodyDef bodyDef = b2DefaultBodyDef();
     bodyDef.position = (b2Vec2){0.f, 0.f};
     b2BodyId Wall = b2CreateBody(worldId, &bodyDef);
     b2ShapeDef shapeDef = b2DefaultShapeDef(); // 默认为静态
     shapeDef.filter.categoryBits = (uint64_t)MyCategories::BORDER_WALL;
     shapeDef.filter.maskBits = (uint64_t)MyCategories::PLAYER |
                                (uint64_t)MyCategories::BULLET |
                                (uint64_t)MyCategories::RESOURCE_BLOCK;
     // up
     s.point1 = (b2Vec2){-w, h};
     s.point2 = (b2Vec2){w, h};
     b2CreateSegmentShape(Wall, &shapeDef, &s);
     // left
     s.point1 = (b2Vec2){w, h};
     s.point2 = (b2Vec2){w, -h};
     b2CreateSegmentShape(Wall, &shapeDef, &s);
     // down
     s.point1 = (b2Vec2){w, -h};
     s.point2 = (b2Vec2){-w, -h};
     b2CreateSegmentShape(Wall, &shapeDef, &s);
     // right
     s.point1 = (b2Vec2){-w, -h};
     s.point2 = (b2Vec2){-w, h};
     b2CreateSegmentShape(Wall, &shapeDef, &s);

     Logger::instance().info("世界" + std::to_string(worldId.index1) + "创建成功");
}

World::~World()
{
     if (b2World_IsValid(worldId))
     {
          b2DestroyWorld(worldId);
          Logger::instance().info("世界" + std::to_string(worldId.index1) + "销毁成功");
     }
     else
     {
          Logger::instance().warn("世界销毁失败：世界可能已通过其他方式销毁");
     }
     worldId = b2_nullWorldId;
}

b2WorldId World::getWorldId()
{
     return worldId;
}

float World::getTimeStep()
{
     return timeStep;
}
void World::updateWorld()
{
     std::lock_guard<std::mutex> lock(worldMutex);
     b2World_Step(worldId, timeStep, 4);
}

Player *World::addPlayer()
{
     std::lock_guard<std::mutex> lock(worldMutex);
     b2BodyDef bodyDef = b2DefaultBodyDef();
     bodyDef.type = b2_dynamicBody;
     bodyDef.position = (b2Vec2){0.f, 0.f};
     bodyDef.linearDamping = 10.0f;

     b2BodyId bodyId = b2CreateBody(worldId, &bodyDef);

     b2ShapeDef shapeDef = b2DefaultShapeDef();
     shapeDef.density = 1.f;   // 默认为1
     shapeDef.friction = 0.1f; // 动态物体需要设置密度和摩擦系数
     shapeDef.filter.categoryBits = (uint64_t)MyCategories::PLAYER;
     shapeDef.filter.maskBits = (uint64_t)MyCategories::BORDER_WALL |
                                (uint64_t)MyCategories::BULLET |
                                (uint64_t)MyCategories::RESOURCE_BLOCK |
                                (uint64_t)MyCategories::CAMERA |
                                (uint64_t)MyCategories::PLAYER;
     shapeDef.filter.groupIndex = negativeGroupIndex--;
     // 圆形
     b2Circle circle;
     circle.center = b2Vec2_zero; // 这个坐标是相对bodyDef.position而言的偏移量
     circle.radius = 0.05f;

     b2CreateCircleShape(bodyId, &shapeDef, &circle);

     Player *player = new Player(100, circle.radius, bodyId, worldMutex);
     player->initGroupIndex(shapeDef.filter.groupIndex);
     b2Body_SetUserData(bodyId, (void *)player);
     return player;
}

void World::removeBody(GameObject *obj)
{
     delete obj;
     obj = nullptr;
}
