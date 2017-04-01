#pragma once
#include <thread>
#include <mutex>
#include <atomic>

class SystemThread
{
public:
	SystemThread();
	~SystemThread();

	void Tick();
	void Shutdown();
	void Pause();
	void Unpause();

private:
	std::thread m_systemThread;
	std::mutex m_systemMutex;
	std::atomic<bool> m_paused;
};

