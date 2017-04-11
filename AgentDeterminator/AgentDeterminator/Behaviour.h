#pragma once
#include <string>

enum FuzzyType {
	FT_SHOULDER_LEFT,
	FT_SHOULDER_RIGHT,
	FT_TRAPEZOID,
	FT_TRIANGULAR
};

class Behaviour
{
public:
	Behaviour(float a_startValue, float a_endValaue);
	Behaviour(float a_minStartValue, float a_maxPeakValue, float a_minEndVlaue);
	Behaviour(float a_minStartValue, float a_maxStartValue, float a_maxEndValue, float a_minEndVlaue);

	~Behaviour();

	void Update();

protected:
	std::string m_sName;
	FuzzyType m_fuzzyType;

	float m_fLeftValue;
	float m_fRightValue;
	float m_fPeakLeftValue;
	float m_fPeakRightValue;

	float m_fCurrentValue;
	float m_fPrevValue;

};

