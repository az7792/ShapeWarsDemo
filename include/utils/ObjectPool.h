#pragma once
#include <iostream>
#include <memory>
#include <queue>
#include <unordered_set>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <type_traits>
#include "Logger.h"

template <typename T>
class ObjectPool
{
public:
     using ObjectFactory = std::function<T *()>;

     /**
      * @brief 初始化对象池，支持自定义对象创建函数
      * @param factory 对象创建函数
      * @param maxSize 最大池大小
      * @param initialSize 初始池大小，默认为0
      */
     ObjectPool(ObjectFactory factory, size_t maxSize, size_t initialSize = 0)
         : maxSize(maxSize), factory(factory)
     {
          initialSize = std::min(initialSize, maxSize);
          for (size_t i = 0; i < initialSize; ++i)
          {
               pool.emplace(factory());
          }
          allocatedCount = initialSize;
     }

     ~ObjectPool()
     {
          assert(pool.size() + used.size() == allocatedCount);
          while (!pool.empty())
          {
               delete pool.front();
               pool.pop();
          }
          for (auto &obj : used)
          {
               delete obj;
          }
     }

     // 获取一个对象
     T *get()
     {
          std::lock_guard<std::mutex> lock(poolMutex);

          if (!pool.empty())
          {
               auto obj = pool.front();
               pool.pop();
               used.insert(obj);
               return obj;
          }

          if (allocatedCount >= maxSize)
               return nullptr; // 池已满
          allocatedCount++;
          auto obj = factory();
          used.insert(obj);
          return obj;
     }

     // 归还一个对象
     void release(T *obj)
     {
          std::lock_guard<std::mutex> lock(poolMutex);
          if (obj)
          {
               pool.emplace(obj);
               used.erase(obj);
          }
     }

private:
     size_t maxSize;               // 最大池大小
     size_t allocatedCount = 0;    // 已经创建数量 = 池中对象数量 + 已经分配出去的数量
     std::queue<T *> pool;         // 池中的空闲对象
     std::unordered_set<T *> used; // 已经分配出去的对象
     ObjectFactory factory;        // 用于创建新对象的工厂函数
     mutable std::mutex poolMutex; // 保护池的访问
};
