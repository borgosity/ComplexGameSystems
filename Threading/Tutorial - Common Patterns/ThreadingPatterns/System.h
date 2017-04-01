#pragma once
#include <chrono>
#include <string>

class System
{
public:
    System() = delete;
    System(std::string systemName, float executeTime, float executeChance = 1.0f);
    ~System();

    void Execute();

    void DisplayAverageFrameTime() const;
    void DisplayAverageFrameRate() const;

    float GetFrameTime() const;
    float GetAverageFrameTime() const;

    float GetFrameRate() const;
    float GetAverageFrameRate() const;
private:
    std::string m_systemName;

    float m_executeTime;
    float m_executeChance;
    float m_frameTime;
    float m_averageFrameTime;
    float m_accumFrameTime;
    int   m_accumFrameCount;
    std::chrono::high_resolution_clock::time_point m_lastExecuteTime;
};
