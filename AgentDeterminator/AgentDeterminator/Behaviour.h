#pragma once
#include <string>

class Behaviour
{
public:
	Behaviour();
	virtual ~Behaviour();

	void virtual update() = 0;
	void virtual destroy() = 0;

protected:
	std::string m_sName;

	float m_fCurrentValue;
	float m_fPrevValue;

	float AND(float a_left, float a_right);
	float OR(float a_left, float a_right);
	float NOT(float a_value);

	template<typename T>
	void deallocate(T a_ptr);
};

template<typename T>
inline void Behaviour::deallocate(T a_ptr)
{
	if (a_ptr != nullptr) {
		delete a_ptr;
		a_ptr = nullptr;
	}
}
