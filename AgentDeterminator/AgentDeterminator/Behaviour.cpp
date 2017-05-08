#include "Behaviour.h"



Behaviour::Behaviour()
{
}

Behaviour::~Behaviour()
{
}

/// AND function retrun the lower value
float Behaviour::AND(float a_left, float a_right)
{
	if (a_left < a_right)
	{
		return a_left;
	}
	else
	{
		return a_right;
	}
}
/// OR function returns the larger value
float Behaviour::OR(float a_left, float a_right)
{
	if (a_left > a_right)
	{
		return a_left;
	}
	else
	{
		return a_right;
	}
}
/// NOT function returns the opposite value
float Behaviour::NOT(float a_value)
{
	return 1 - a_value;
}

void Behaviour::saveHistory(float a_currVal)
{
	if (a_currVal > 0) {
		if (traits.history.size() < traits.maxHistory) {
			traits.history.push_back(a_currVal);
		}
		else {
			traits.history.pop_front();
			traits.history.push_back(a_currVal);
		}
	}
}
