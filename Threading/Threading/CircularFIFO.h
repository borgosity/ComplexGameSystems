#pragma once
#include <iostream>
#include <atomic>

template <class QType, size_t QSize> class CircularFIFO
{
public:
	CircularFIFO() : m_readLocation(0), m_writeLocation(0), QSIZE(QSize + 1) {};
	~CircularFIFO() {};

	bool Push(const QType & a_item);
	bool Pop(QType & a_item);

	bool IsEmpty() const;
	bool IsFull() const;

private:
	size_t Increment(size_t index) const;

	std::atomic<size_t> m_readLocation;
	std::atomic<size_t> m_writeLocation;

	size_t			QSIZE;
	QType			m_data[QSize +1];
};
/**********************************************************************************
* Push item on to queue
***********************************************************************************/
template<class QType, size_t QSize>
bool CircularFIFO<QType, QSize>::Push(const QType & a_item)
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
/**********************************************************************************
* Pull item off queue
***********************************************************************************/
template<class QType, size_t QSize>
bool CircularFIFO<QType, QSize>::Pop(QType & a_item)
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
/**********************************************************************************
* Check if queue is empty, returns true if empty
***********************************************************************************/
template<class QType, size_t QSize>
bool CircularFIFO<QType, QSize>::IsEmpty() const
{
	// if both read an d write locations are the same, then queue is empty
	return (m_readLocation.load() == m_writeLocation.load());
}
/**********************************************************************************
* Check if queue is full, retruns true if full
***********************************************************************************/
template<class QType, size_t QSize>
bool CircularFIFO<QType, QSize>::IsFull() const
{
	const auto nextWriteLocation = Increment(m_writeLocation.load());
	return (nextWriteLocation == m_readLocation.load());
}
/**********************************************************************************
* 
***********************************************************************************/
template<class QType, size_t QSize>
size_t CircularFIFO<QType, QSize>::Increment(size_t index) const
{
	// if index reaches QSIZE set loop back to 0
	return (index + 1) % (QSIZE);
}
