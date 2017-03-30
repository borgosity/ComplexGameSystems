#include "MultiThreadedApplication.h"

MultiThreadedApplication::MultiThreadedApplication() : m_waitingToWriteData(false)
{
	// path finding thread
	m_pathFindingThread = std::thread([&] {
		while (true) {
			m_pathfinding.Execute();
		}
	});
	// rendering thread
	m_renderThread = std::thread([&] {
		while (true) {
			if (!m_waitingToWriteData) {
				m_renderDataMutex.lock();
				m_culling.Execute();
				m_renderer.Execute();
				m_renderDataMutex.unlock();
			}
		}
	});
}

MultiThreadedApplication::~MultiThreadedApplication()
{
}

void MultiThreadedApplication::Tick()
{
	m_input.Execute();
	m_gameLogic.Execute();
	m_physics.Execute();
	// ---------------------------------------
	// path finding occurs in contructor loop
	// ---------------------------------------

	// Update render data Must be called before rendering can occur!
	m_waitingToWriteData = true;	// make sure rendering doesn't start before lock established
	m_renderDataMutex.lock();		// get lock
	m_waitingToWriteData = false;	// allow rendering to occur
	m_updateRenderData.Execute();	// update render data
	m_renderDataMutex.unlock();		// unlock

	// ---------------------------------------
	// rendering occurs in contructor loop
	// ---------------------------------------

}
