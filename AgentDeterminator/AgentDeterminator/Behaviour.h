#pragma once
#include <string>

enum FuzzyMemberType {
	FMT_SHOULDER_LEFT,
	FMT_SHOULDER_RIGHT,
	FMT_TRAPEZOID,
	FMT_TRIANGULAR,
	FMT_GAUSIAN,
	FMT_SINGLETON
};

class Behaviour
{
public:
	Behaviour();
	Behaviour(FuzzyMemberType	a_fuzzyMemberType);
	Behaviour(float a_startValue, float a_endValaue);
	Behaviour(float a_minStartValue, float a_maxPeakValue, float a_minEndVlaue);
	Behaviour(float a_minStartValue, float a_maxStartValue, float a_maxEndValue, float a_minEndVlaue);

	~Behaviour();

	void Update();

protected:
	std::string m_sName;
	FuzzyMemberType m_fuzzyMemberType;

	float m_fLeftValue;
	float m_fRightValue;
	float m_fPeakLeftValue;
	float m_fPeakRightValue;

	float m_fCurrentValue;
	float m_fPrevValue;

};

