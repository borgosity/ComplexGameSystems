#pragma once
#include <iostream>
#include <atomic>

template <class QType, size_t QSize> class CircularFIFO
{
public:
	CircularFIFO();
	CircularFIFO(int a_queueSize);
	~CircularFIFO();

	bool Push(const int& a_item);
	bool Pop(int& a_item);

	bool IsEmpty() const;
	bool IsFull() const;

private:
	size_t Increment(size_t index) const;

	std::atomic<size_t> m_readLocation;
	std::atomic<size_t> m_writeLocation;

	size_t			QSIZE;
	QType			m_data[QSize];
};

