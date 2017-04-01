#pragma once
#include <thread>

#include "System.h"

class BaseApplication
{
public:
    BaseApplication();
    virtual ~BaseApplication() {}

    virtual void Tick() = 0;

protected:
    System m_culling;
    System m_renderer;
    System m_input;
    System m_gameLogic;
    System m_physics;
    System m_pathfinding;
    System m_updateRenderData;
private:
    std::thread m_displayTimeThread;
};
