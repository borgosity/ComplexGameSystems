#include "FuzzYMemberSets.h"



LeftShoulderTriangularRightShoulder::LeftShoulderTriangularRightShoulder(float a_leftShoulderMin, float a_leftShoulderMax, float triangularMin, float triangularPeak, float triangularMax, float a_rightShoulderMin, float a_rightShoulderMax)
{
	// initialise membership functions
	m_leftShoulder = new FMF_LeftShoulder(a_leftShoulderMin, a_leftShoulderMax);
	m_triangular = new FMF_Triangular(triangularMin, triangularPeak, triangularMax);
	m_rightShoulder = new FMF_RightShoulder(a_rightShoulderMin, a_rightShoulderMax);
	// get settings
	std::vector<float> left = m_leftShoulder->settings();
	std::vector<float> tri = m_triangular->settings();
	std::vector<float> right = m_rightShoulder->settings();
	// save settings
	m_fvSettings.insert(m_fvSettings.end(), left.begin(), left.end());
	m_fvSettings.insert(m_fvSettings.end(), tri.begin(), tri.end());
	m_fvSettings.insert(m_fvSettings.end(), right.begin(), right.end());
}

LeftShoulderTriangularRightShoulder::~LeftShoulderTriangularRightShoulder()
{
	destroy();
}

void LeftShoulderTriangularRightShoulder::update(Agent & a_agent)
{
	doms.leftShoulder = m_leftShoulder->membership(a_agent.vitals.currentDistance);
	doms.triangular = m_triangular->membership(a_agent.vitals.currentDistance);
	doms.rightShoulder = m_rightShoulder->membership(a_agent.vitals.currentDistance);
}

/// Updates Values of the Membership Functions
/// takes a vector size 7, 
/// - 2 floats for left shoulder
/// - 3 floats for triangular
/// - 2 floats for right shoulder
std::vector<float> LeftShoulderTriangularRightShoulder::settings(std::vector<float> a_settings)
{
	// clear settings
	m_fvSettings.empty();
	// set new settings
	std::vector<float> left = m_leftShoulder->settings(a_settings);
	std::vector<float> tri = m_triangular->settings(a_settings);
	std::vector<float> right = m_rightShoulder->settings(a_settings);
	// save settings
	m_fvSettings.insert(m_fvSettings.end(), left.begin(), left.end());
	m_fvSettings.insert(m_fvSettings.end(), tri.begin(), tri.end());
	m_fvSettings.insert(m_fvSettings.end(), right.begin(), right.end());

	return m_fvSettings;
}
/// cleanup membership functions
void LeftShoulderTriangularRightShoulder::destroy()
{
	deallocate(m_leftShoulder);
	deallocate(m_triangular);
	deallocate(m_rightShoulder);
}
