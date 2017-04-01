#pragma once
#include <iostream>
#include <atomic>

class IntCircularFIFO10
{
public:
	IntCircularFIFO10();
	~IntCircularFIFO10();

	bool Push(const int& a_item);
	bool Pop(int& a_item);

	bool IsEmpty() const;
	bool IsFull() const;

private:
	size_t Increment(size_t index) const;

	std::atomic<size_t> m_readLocation;
	std::atomic<size_t> m_writeLocation;

	static const int SIZE = 11;
	int				 m_data[SIZE];

};

