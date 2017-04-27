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
