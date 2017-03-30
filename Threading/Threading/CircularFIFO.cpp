#include "CircularFIFO.h"


template<class QType, size_t QSize>
CircularFIFO<QType, QSize>::CircularFIFO() : m_readLocation(0), m_writeLocation(0), QSIZE(QSize +1)
{
}

//template<class QType, size_t QSize>>
//CircularFIFO<QType, QSize>>::CircularFIFO(int a_queueSize)
//{
//	m_readLocation = 0;
//	m_writeLocation = 0;
//	QSIZE = a_queueSize +1;
//}

template<class QType, size_t QSize>
CircularFIFO<QType, QSize>::~CircularFIFO()
{
}

template<class QType, size_t QSize>
bool CircularFIFO<QType, QSize>::Push(const int & a_item)
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
template<class QType, size_t QSize>
bool CircularFIFO<QType, QSize>::Pop(int & a_item)
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
template<class QType, size_t QSize>
bool CircularFIFO<QType, QSize>::IsEmpty() const
{
	// if both read an d write locations are the same, then queue is empty
	return (m_readLocation.load() == m_writeLocation.load());
}
template<class QType, size_t QSize>
bool CircularFIFO<QType, QSize>::IsFull() const
{
	const auto nextWriteLocation = Increment(m_writeLocation.load());
	return (nextWriteLocation == m_readLocation.load());
}
template<class QType, size_t QSize>
size_t CircularFIFO<QType, QSize>::Increment(size_t index) const
{
	// if index reaches QSIZE set loop back to 0
	return (index + 1) % (QSIZE);
}

