#include "BaseApplication.h"
#include <iostream>
BaseApplication::BaseApplication()
    : m_culling("Culling", 0.0025f)
    , m_renderer("Renderer", 0.0042f)
    , m_input("Input", 0.0005f)
    , m_gameLogic("Game Logic", 0.004f)
    , m_physics("Physics", 0.006f)
    , m_pathfinding("Pathfinder", 0.2f, 0.1f)
    , m_updateRenderData("Update Renderer", 0.001f)
{
    m_displayTimeThread = std::thread([&] {
        while(true) 
        {
            system("cls");
            std::cout << "Frame times:" << std::endl;
            std::cout << "-----------" << std::endl;
            m_updateRenderData.DisplayAverageFrameTime();
            m_culling.DisplayAverageFrameTime();
            m_renderer.DisplayAverageFrameTime();
            m_input.DisplayAverageFrameTime();
            m_gameLogic.DisplayAverageFrameTime();
            m_physics.DisplayAverageFrameTime();
            m_pathfinding.DisplayAverageFrameTime();
            std::cout << "\n\n";

            std::cout << "Frame rates" << std::endl;
            std::cout << "-----------" << std::endl;
            m_updateRenderData.DisplayAverageFrameRate();
            m_culling.DisplayAverageFrameRate();
            m_renderer.DisplayAverageFrameRate();
            m_input.DisplayAverageFrameRate();
            m_gameLogic.DisplayAverageFrameRate();
            m_physics.DisplayAverageFrameRate();
            m_pathfinding.DisplayAverageFrameRate();

            std::this_thread::sleep_for(std::chrono::duration<float>(1.0f));
        }
    });
}
