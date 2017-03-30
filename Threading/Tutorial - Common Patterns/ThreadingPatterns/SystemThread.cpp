#include "SystemThread.h"



SystemThread::SystemThread() : m_paused(false) , m_systemThread()
{

}


SystemThread::~SystemThread()
{
}

void SystemThread::Tick()
{
	m_systemThread = std::thread([&] {
		if (!m_paused) {
			// do something
			m_systemThread = std::thread(&SystemThread::Tick, this);
		}
	});
}

void SystemThread::Shutdown()
{
	if (m_systemThread.joinable()) {
		m_systemThread.join();
	}
}

void SystemThread::Pause()
{
	m_paused = true;
}

void SystemThread::Unpause()
{
	m_paused = false;
}
