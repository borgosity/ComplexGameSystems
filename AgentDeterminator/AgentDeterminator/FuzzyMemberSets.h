#pragma once
#include "FuzzyMemberTypes.h"


class Agent;

struct DegreeOfMembership
{
	float leftShoulder;
	float rightShoulder;
	float triangular;
	float trapezoid;
};

struct MaxMembership
{
	float leftShoulder;
	float rightShoulder;
	float triangular;
	float trapezoid;
};

class LeftShoulderTriangularRightShoulder
{
public:
	LeftShoulderTriangularRightShoulder(float a_leftShoulderMin, float a_leftShoulderMax, 
										float triangularMin, float triangularPeak, float triangularMax, 
										float a_rightShoulderMin, float a_rightShoulderMax, std::string a_name);
	LeftShoulderTriangularRightShoulder(std::vector<float> a_values, std::string a_name);
	virtual ~LeftShoulderTriangularRightShoulder();

	void update(Agent & a_agent, float a_input);
	void drawGUI();
	DegreeOfMembership doms;
	MaxMembership maxDom;

	// get fuzzy settings
	std::vector<float> settings() { return m_fvSettings; };
	// set fuzzy settings
	std::vector<float> settings(std::vector<float> a_settings);
	// cleanup
	void destroy();
private:
	std::string m_name;
	// fuzzy settings
	std::vector<float> m_fvSettings;
	// distance
	FMF_LeftShoulder	* m_leftShoulder = nullptr;
	FMF_Triangular		* m_triangular = nullptr;
	FMF_RightShoulder	* m_rightShoulder = nullptr;
	// math
	float lineHeight(float rightValue, float leftValue);
	// cleanup
	template<typename T>
	void deallocate(T a_ptr);
};

template<typename T>
inline void LeftShoulderTriangularRightShoulder::deallocate(T a_ptr)
{
	if (a_ptr != nullptr) {
		delete a_ptr;
		a_ptr = nullptr;
	}
}
