#include "IntCircularFIFO10.h"



IntCircularFIFO10::IntCircularFIFO10() : m_readLocation(0), m_writeLocation(0)
{
}


IntCircularFIFO10::~IntCircularFIFO10()
{
}

bool IntCircularFIFO10::Push(const int & a_item)
{
	auto currentWriteLocation = m_writeLocation.load();
	auto nextWriteLocation = Increment(currentWriteLocation);
	// if queues NOT full write to queue
	if (nextWriteLocation != m_readLocation.load()) {
		m_data[currentWriteLocation] = a_item;
		m_writeLocation.store(nextWriteLocation);
		return true;
	}
	// full queue
	std::cout << " PUSH! >> QUEUE FULL <<" << std::endl;
	return false;
}

bool IntCircularFIFO10::Pop(int & a_item)
{
	auto currentReadLocation = m_readLocation.load();
	// if queue is empty return false
	if (currentReadLocation == m_writeLocation.load()) {
		std::cout << " POP! >> QUEUE EMPTY <<" << std::endl;
		return false;
	}
	// if queue isn't empty
	a_item = m_data[currentReadLocation];
	m_readLocation.store(Increment(currentReadLocation));
	return true;
}

bool IntCircularFIFO10::IsEmpty() const
{
	// if both read an d write locations are the same, then queue is empty
	return (m_readLocation.load() == m_writeLocation.load());
}

bool IntCircularFIFO10::IsFull() const
{
	const auto nextWriteLocation = Increment(m_writeLocation.load());
	return (nextWriteLocation == m_readLocation.load());
}

size_t IntCircularFIFO10::Increment(size_t index) const
{
	// if index reaches SIZE set loop back to 0
	return (index + 1) % (SIZE);
}
