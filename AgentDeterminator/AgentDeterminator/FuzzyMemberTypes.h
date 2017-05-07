#pragma once
#include <string>
#include <vector>

enum FuzzyMemberType {
	FMT_SHOULDER_LEFT,
	FMT_SHOULDER_RIGHT,
	FMT_TRAPEZOID,
	FMT_TRIANGULAR,
	FMT_GAUSIAN,
	FMT_SINGLETON
};
struct FuzzyMFValues {
	std::string name = "default";
	float start = 0.0f;
	float peak = 0.0f;
	float peakB = 0.0f;
	float end = 0.0f;
};
/*************************************************************************
* Base Membership Class
***************************************************************************/
class FuzzyMemberFunction {
public:
	virtual ~FuzzyMemberFunction();
	FuzzyMFValues graph;

protected:
	// member function values stored in left to right membership graph order
	std::vector<float> m_values;
	// membership functions
	float virtual membership(float a_value) = 0;
	float virtual maxMembership() = 0;
	// fuzzy settings get/set
	std::vector<float> virtual settings() = 0;
	std::vector<float> virtual settings(std::vector<float> a_values) = 0;
};
/*************************************************************************
* Left SHoulder Membership Class
***************************************************************************/
class FMF_LeftShoulder : public FuzzyMemberFunction {
public:
	FMF_LeftShoulder(float a_startValue, float a_endValaue);
	virtual ~FMF_LeftShoulder();

	//FuzzyMFValues graph;
	// membership functions
	float virtual membership(float a_value);
	float virtual maxMembership();
	// fuzzy settings get/set
	std::vector<float> virtual settings() { return m_values; };
	std::vector<float> virtual settings(std::vector<float> a_values);

};
/*************************************************************************
* Right Shoulder Membership Class
***************************************************************************/
class FMF_RightShoulder : public FuzzyMemberFunction {
public:
	FMF_RightShoulder(float a_startValue, float a_endValaue);
	virtual ~FMF_RightShoulder();
	// membership functions
	float virtual membership(float a_value);
	float virtual maxMembership();
	// fuzzy settings get/set
	std::vector<float> virtual settings() { return m_values; };
	std::vector<float> virtual settings(std::vector<float> a_values);

};
/*************************************************************************
* Triangular Membership Class
***************************************************************************/
class FMF_Triangular : public FuzzyMemberFunction {
public:
	FMF_Triangular(float a_minStartValue, float a_maxPeakValue, float a_minEndVlaue);
	virtual ~FMF_Triangular();
	// membership functions
	float virtual membership(float a_value);
	float virtual maxMembership();
	// fuzzy settings get/set
	std::vector<float> virtual settings() { return m_values; };
	std::vector<float> virtual settings(std::vector<float> a_values);

};
/*************************************************************************
* Trapazoid Membership Class
***************************************************************************/
class FMF_Trapazoid : public FuzzyMemberFunction {
public:
	FMF_Trapazoid(float a_minStartValue, float a_maxStartValue, float a_maxEndValue, float a_minEndVlaue);
	virtual ~FMF_Trapazoid();
	// membership functions
	float virtual membership(float a_value);
	float virtual maxMembership();
	// fuzzy settings get/set
	std::vector<float> virtual settings() { return m_values; };
	std::vector<float> virtual settings(std::vector<float> a_values);

};