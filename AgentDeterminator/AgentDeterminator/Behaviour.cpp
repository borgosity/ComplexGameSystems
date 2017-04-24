#include "Behaviour.h"



Behaviour::Behaviour()
{
}

Behaviour::Behaviour(FuzzyMemberType a_fuzzyMemberType)
{
	switch (a_fuzzyMemberType)
	{
	case FMT_SHOULDER_LEFT:
		break;
	case FMT_SHOULDER_RIGHT:
		break;
	case FMT_TRAPEZOID:
		break;
	case FMT_TRIANGULAR:
		break;
	case FMT_GAUSIAN:
		break;
	case FMT_SINGLETON:
		break;
	default:
		break;
	}
}


Behaviour::Behaviour(float a_startValue, float a_endValaue)
{
}

Behaviour::Behaviour(float a_minStartValue, float a_maxPeakValue, float a_minEndVlaue)
{
}

Behaviour::Behaviour(float a_minStartValue, float a_maxStartValue, float a_maxEndValue, float a_minEndVlaue)
{
}

Behaviour::~Behaviour()
{
}

void Behaviour::Update()
{
}
