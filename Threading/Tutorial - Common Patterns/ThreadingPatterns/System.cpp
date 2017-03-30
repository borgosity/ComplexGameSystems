#include "System.h"
#include <chrono>
#include <random>
#include <iostream>
#include <iterator>

System::System(std::string systemName, float executeTime, float executeChance /* = 1.0f */)
    :   m_systemName(systemName)
    ,   m_executeTime(executeTime)
    ,   m_executeChance(executeChance)
    ,   m_accumFrameTime(0)
    ,   m_accumFrameCount(0)
    ,   m_averageFrameTime(-1.0f)
    ,   m_frameTime(0)
{
    m_lastExecuteTime = std::chrono::high_resolution_clock::now();
}

System::~System()
{

}

void System::Execute()
{
    auto startTime = std::chrono::high_resolution_clock::now();
    auto endTime = startTime + std::chrono::duration<double>(m_executeTime);
    
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<float> dist(0.0f, 1.0f);

    auto currentTime = startTime;

    if (dist(gen) < m_executeChance)
    {
        //Simulate work being done...
        for (; currentTime < endTime; currentTime = std::chrono::high_resolution_clock::now()) {}
    }

 
    std::chrono::duration<float> time = currentTime - m_lastExecuteTime;
    m_frameTime = time.count();
    m_lastExecuteTime = currentTime;

    m_accumFrameTime += m_frameTime;
    ++m_accumFrameCount;

    if (m_accumFrameTime > 1.0f)
    {
        m_averageFrameTime = m_accumFrameTime / (float)m_accumFrameCount;
        m_accumFrameCount = 0;
        m_accumFrameTime = 0.0f;
    }
}

void System::DisplayAverageFrameTime() const
{
    std::cout << m_systemName << ":";
    std::fill_n(std::ostream_iterator<char>(std::cout), 15 - m_systemName.length(), ' ');
    std::cout << GetAverageFrameTime() << std::endl;
}


void System::DisplayAverageFrameRate() const
{
    std::cout << m_systemName << ":";
    std::fill_n(std::ostream_iterator<char>(std::cout), 15 - m_systemName.length(), ' ');
    std::cout << GetAverageFrameRate() << std::endl;
}

float System::GetFrameTime() const
{
    return m_frameTime;
}

float System::GetAverageFrameTime() const
{
    return m_averageFrameTime;
}

float System::GetFrameRate() const
{
    return 1.0f / m_frameTime;
}

float System::GetAverageFrameRate() const
{
    return 1.0f / m_averageFrameTime;
}
