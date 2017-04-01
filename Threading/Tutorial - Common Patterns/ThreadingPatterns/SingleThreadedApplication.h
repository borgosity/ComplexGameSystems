#pragma once

#include "BaseApplication.h"

class SingleThreadedApplication : public BaseApplication
{
public:
    SingleThreadedApplication() = default;
    ~SingleThreadedApplication() = default;

    void Tick() override;
};

