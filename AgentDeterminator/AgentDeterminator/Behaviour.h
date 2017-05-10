#pragma once
#include <string>
#include <vector>
#include <list>
#include"Action.h"

class Agent;
class LeftShoulderTriangularRightShoulder;

struct BehaviourTraits
{
	std::string name = "No Name";
	ActionType action;
	float priority = 1.0f;
	float currWeight = 0.0f;
	float prevWeight = 0.0f;
	float maxHistory = 50.0f;
	std::list<float> history = {0.0f};
	bool showSets = false;
};

class Behaviour
{
public:
	Behaviour();
	virtual ~Behaviour();

	void virtual update(Agent & a_agent) = 0;
	void drawGUI(Agent & a_agent);
	void virtual destroy() = 0;
	std::vector<float> virtual desireSettings() = 0;
	BehaviourTraits traits;
	
protected:
	// protected variables
	std::vector<LeftShoulderTriangularRightShoulder*> m_memberSets;
	// protected functions
	float AND(float a_left, float a_right);
	float OR(float a_left, float a_right);
	float NOT(float a_value);

	void saveHistory(float a_currVal);
	std::vector<float> genBroadLTRSet(float a_min, float a_max);
	std::vector<float> genSteepLTRSet(float a_min, float a_max);

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
