#include "SingleThreadedApplication.h"

void SingleThreadedApplication::Tick()
{
    m_input.Execute();
    m_gameLogic.Execute();
    m_physics.Execute();
    m_pathfinding.Execute();

    //Must be called before rendering can occur!
    m_updateRenderData.Execute();

    m_culling.Execute();
    m_renderer.Execute();
}
