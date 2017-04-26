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

};

