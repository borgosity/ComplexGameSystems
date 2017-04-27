#pragma once
#include <string>

class Behaviour
{
public:
	Behaviour();
	Behaviour(std::string a_name);

	~Behaviour();

	void Update();

protected:
	std::string m_sName;

	float m_fCurrentValue;
	float m_fPrevValue;

	float AND(float a_left, float a_right);
	float OR(float a_left, float a_right);
	float NOT(float a_value);

};

