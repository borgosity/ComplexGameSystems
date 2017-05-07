#include "FuzzyMemberTypes.h"


FuzzyMemberFunction::~FuzzyMemberFunction()
{
}


/**********************************************************************************************************
* Left Shoulder Functions
***********************************************************************************************************/
FMF_LeftShoulder::FMF_LeftShoulder(float a_startValue, float a_endValaue)
{
	graph.name = "Left Shoulder";
	graph.start = a_startValue;
	graph.end = a_endValaue;
	// values vector
	m_values.push_back(a_startValue);
	m_values.push_back(a_endValaue);
}
FMF_LeftShoulder::~FMF_LeftShoulder()
{
}
/// Returns set membership value
float FMF_LeftShoulder::membership(float a_value)
{
	float result = 0;
	if (a_value <= graph.start) {
		result = 1;
	}
	else if (a_value >= graph.end) {
		result = 0;
	}
	else {
		result = (-a_value / (graph.end - graph.start)) + (graph.end / (graph.end - graph.start));
	}
	return result;
}
/// Returns maximum membership value
float FMF_LeftShoulder::maxMembership()
{
	return (graph.start) * 0.5f;
}
std::vector<float> FMF_LeftShoulder::settings(std::vector<float> a_values)
{
	// vector values
	m_values = a_values;
	// float values
	graph.start = m_values[0];
	graph.end = m_values[1];

	return m_values;
}
/**********************************************************************************************************
* Right Shoulder Functions
***********************************************************************************************************/
FMF_RightShoulder::FMF_RightShoulder(float a_startValue, float a_endValaue)
{
	graph.name = "Right Shoulder";
	graph.start = a_startValue;
	graph.end = a_endValaue;
	// values vector
	m_values.push_back(a_startValue);
	m_values.push_back(a_endValaue);
}
FMF_RightShoulder::~FMF_RightShoulder()
{
}
/// Returns set membership value
float FMF_RightShoulder::membership(float a_value)
{
	float result = 0;
	if (a_value <= graph.start) {
		result = 0;
	}
	else if (a_value >= graph.end) {
		result = 1;
	}
	else {
		result = (a_value / (graph.end - graph.start)) - (graph.start / (graph.end - graph.start));
	}
	return result;
}
/// Returns maximum membership value
float FMF_RightShoulder::maxMembership()
{
	return (graph.start + 1) * 0.5f;
}
std::vector<float> FMF_RightShoulder::settings(std::vector<float> a_values)
{
	// vector values
	m_values = a_values;
	// float values
	graph.start = m_values[0];
	graph.end = m_values[1];

	return m_values;
}
/**********************************************************************************************************
* Triangular Functions
***********************************************************************************************************/
FMF_Triangular::FMF_Triangular(float a_minStartValue, float a_maxPeakValue, float a_minEndVlaue)
{
	graph.name = "Triangular";
	graph.start = a_minStartValue;
	graph.peak = a_maxPeakValue;
	graph.end = a_minEndVlaue;
	// values vector
	m_values.push_back(a_minStartValue);
	m_values.push_back(a_maxPeakValue);
	m_values.push_back(a_minEndVlaue);
}
FMF_Triangular::~FMF_Triangular()
{
}
/// Returns set membership value
float FMF_Triangular::membership(float a_value)
{
	float result = 0;
	if (a_value <= graph.start || a_value >= graph.end) {
		result = 0;
	}
	else if (a_value == graph.peak) {
		result = 1;
	}
	else if ((a_value > graph.start) && (a_value < graph.peak)) {
		result = (a_value / (graph.peak - graph.start)) - (graph.start / (graph.peak - graph.start));
	}
	else {
		result = (-a_value / (graph.end - graph.peak)) + (graph.end / (graph.end - graph.peak));
	}
	return result;
}
/// Returns maximum membership value
float FMF_Triangular::maxMembership()
{
	return graph.peak;
}
std::vector<float> FMF_Triangular::settings(std::vector<float> a_values)
{
	// vector values
	m_values = a_values;
	// float values
	graph.start = m_values[0];
	graph.peak = m_values[1];
	graph.end = m_values[2];

	return m_values;
}
/**********************************************************************************************************
* Trapazoid Functions
***********************************************************************************************************/
FMF_Trapazoid::FMF_Trapazoid(float a_minStartValue, float a_maxStartValue, float a_maxEndValue, float a_minEndVlaue)
{
	graph.name = "Trapazoid";
	graph.start = a_minStartValue;
	graph.peak = a_maxStartValue;
	graph.peakB = a_maxEndValue;
	graph.end = a_minEndVlaue;

	// values vector
	m_values.push_back(a_minStartValue);
	m_values.push_back(a_maxStartValue);
	m_values.push_back(a_maxEndValue);
	m_values.push_back(a_minEndVlaue);
}
FMF_Trapazoid::~FMF_Trapazoid()
{
}
/// Returns set membership value
float FMF_Trapazoid::membership(float a_value)
{
	float result = 0;
	if (a_value <= graph.start || a_value >= graph.end) {
		result = 0;
	}
	else if ((a_value >= graph.peak) && (a_value <= graph.peakB)) {
		result = 1;
	}
	else if ((a_value > graph.start) && (a_value < graph.peak)) {
		result = (a_value / (graph.peak - graph.start)) - (graph.start / (graph.peak - graph.start));
	}
	else {
		result = (-a_value / (graph.end - graph.peakB)) + (graph.end / (graph.end - graph.peakB));
	}
	return result;
}
/// Returns maximum membership value
float FMF_Trapazoid::maxMembership()
{
	return (graph.peak + graph.peakB) * 0.5f;
}

std::vector<float> FMF_Trapazoid::settings(std::vector<float> a_values)
{
	// vector values
	m_values = a_values;
	// float values
	graph.start = m_values[0];
	graph.peak = m_values[1];
	graph.peakB = m_values[2];
	graph.end = m_values[3];

	return m_values;
}