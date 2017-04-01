#pragma once
#include <thread>
#include <mutex>
#include <atomic>

#include "BaseApplication.h"


class MultiThreadedApplication :
	public BaseApplication
{
public:
	MultiThreadedApplication();
	~MultiThreadedApplication();

	void Tick() override;

private:
	std::thread m_renderThread;
	std::mutex m_renderDataMutex;
	std::atomic<bool> m_waitingToWriteData;

	std::thread m_pathFindingThread;
};

