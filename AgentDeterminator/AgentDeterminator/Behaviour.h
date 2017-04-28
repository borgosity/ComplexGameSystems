#pragma once
#include <string>

class Agent;

struct BehaviourTraits
{
	std::string name;
	float priority;
	float currWeight;
	float prevWeight;
};


class Behaviour
{
public:
	Behaviour();
	virtual ~Behaviour();

	void virtual update(Agent & a_agent) = 0;
	void virtual destroy() = 0;

	BehaviourTraits traits;

protected:

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
