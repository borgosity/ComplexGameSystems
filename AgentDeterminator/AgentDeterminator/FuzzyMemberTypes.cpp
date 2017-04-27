#include "FuzzyMemberTypes.h"


/**********************************************************************************************************
* Left Shoulder Functions
***********************************************************************************************************/
FMF_LeftShoulder::FMF_LeftShoulder(float a_startValue, float a_endValaue)
{
	m_fLeftValue = a_startValue;
	m_fPeakLeftValue = a_endValaue;
}
/// Returns set membership value
float FMF_LeftShoulder::membership(float a_value)
{
	float result = 0;
	if (a_value <= m_fLeftValue) {
		result = 1;
	}
	else if (a_value >= m_fPeakLeftValue) {
		result = 0;
	}
	else {
		result = (-a_value / (m_fPeakLeftValue - m_fLeftValue)) + (m_fPeakLeftValue / (m_fPeakLeftValue - m_fLeftValue));
	}
	return result;
}
/// Returns maximum membership value
float FMF_LeftShoulder::maxMembership()
{
	return (m_fLeftValue) * 0.5f;
}
/**********************************************************************************************************
* Right Shoulder Functions
***********************************************************************************************************/
FMF_RightShoulder::FMF_RightShoulder(float a_startValue, float a_endValaue)
{
	m_fRightValue = a_startValue;
	m_fPeakRightValue = a_endValaue;
}
/// Returns set membership value
float FMF_RightShoulder::membership(float a_value)
{
	float result = 0;
	if (a_value <= m_fRightValue) {
		result = 0;
	}
	else if (a_value >= m_fPeakRightValue) {
		result = 1;
	}
	else {
		result = (a_value / (m_fPeakRightValue - m_fRightValue)) - (m_fRightValue / (m_fPeakRightValue - m_fRightValue));
	}
	return result;
}
/// Returns maximum membership value
float FMF_RightShoulder::maxMembership()
{
	return (m_fRightValue + 1) * 0.5f;
}
/**********************************************************************************************************
* Triangular Functions
***********************************************************************************************************/
FMF_Triangular::FMF_Triangular(float a_minStartValue, float a_maxPeakValue, float a_minEndVlaue)
{
	m_fLeftValue = a_minStartValue;
	m_fPeakValue = a_maxPeakValue;
	m_fRightValue = a_minEndVlaue;
}
/// Returns set membership value
float FMF_Triangular::membership(float a_value)
{
	float result = 0;
	if (a_value <= m_fLeftValue || a_value >= m_fRightValue) {
		result = 0;
	}
	else if (a_value == m_fPeakValue) {
		result = 1;
	}
	else if ((a_value > m_fLeftValue) && (a_value < m_fPeakValue)) {
		result = (a_value / (m_fPeakValue - m_fLeftValue)) - (m_fLeftValue / (m_fPeakValue - m_fLeftValue));
	}
	else {
		result = (-a_value / (m_fRightValue - m_fPeakValue)) + (m_fRightValue / (m_fRightValue - m_fPeakValue));
	}
	return result;
}
/// Returns maximum membership value
float FMF_Triangular::maxMembership()
{
	return m_fPeakValue;
}
/**********************************************************************************************************
* Trapazoid Functions
***********************************************************************************************************/
FMF_Trapazoid::FMF_Trapazoid(float a_minStartValue, float a_maxStartValue, float a_maxEndValue, float a_minEndVlaue)
{
	m_fLeftValue = a_minStartValue;
	m_fPeakLeftValue = a_maxStartValue;
	m_fPeakRightValue = a_maxEndValue;
	m_fRightValue = a_minEndVlaue;
}
/// Returns set membership value
float FMF_Trapazoid::membership(float a_value)
{
	float result = 0;
	if (a_value <= m_fLeftValue || a_value >= m_fRightValue) {
		result = 0;
	}
	else if ((a_value >= m_fPeakLeftValue) && (a_value <= m_fPeakRightValue)) {
		result = 1;
	}
	else if ((a_value > m_fLeftValue) && (a_value < m_fPeakLeftValue)) {
		result = (a_value / (m_fPeakLeftValue - m_fLeftValue)) - (m_fLeftValue / (m_fPeakLeftValue - m_fLeftValue));
	}
	else {
		result = (-a_value / (m_fRightValue - m_fPeakRightValue)) + (m_fRightValue / (m_fRightValue - m_fPeakRightValue));
	}
	return result;
}
/// Returns maximum membership value
float FMF_Trapazoid::maxMembership()
{
	return (m_fPeakLeftValue + m_fPeakRightValue) * 0.5f;
}
