#include "FuzzYMemberSets.h"
#include "Agents.h"
#include "imgui_glfw3.h"

LeftShoulderTriangularRightShoulder::LeftShoulderTriangularRightShoulder(float a_leftShoulderMin, float a_leftShoulderMax, float triangularMin, float triangularPeak, float triangularMax, float a_rightShoulderMin, float a_rightShoulderMax, std::string a_name)
{
	m_name = a_name;
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

LeftShoulderTriangularRightShoulder::LeftShoulderTriangularRightShoulder(std::vector<float> a_values, std::string a_name)
{
	m_name = a_name;
	// initialise membership functions
	m_leftShoulder = new FMF_LeftShoulder(a_values[0], a_values[1]);
	m_triangular = new FMF_Triangular(a_values[2], a_values[3], a_values[4]);
	m_rightShoulder = new FMF_RightShoulder(a_values[5], a_values[6]);
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

void LeftShoulderTriangularRightShoulder::update(Agent & a_agent, float a_input)
{
	// degrees of membership
	doms.leftShoulder = m_leftShoulder->membership(a_input);
	doms.triangular = m_triangular->membership(a_input);
	doms.rightShoulder = m_rightShoulder->membership(a_input);
	// max membership
	maxDom.leftShoulder = m_leftShoulder->maxMembership();
	maxDom.triangular = m_triangular->maxMembership();
	maxDom.rightShoulder = m_rightShoulder->maxMembership();
}

void LeftShoulderTriangularRightShoulder::drawGUI()
{
	// create agent window
	std::vector<float> values = { 1.0f,lineHeight(m_leftShoulder->graph.end, m_triangular->graph.start),
									1.0f,lineHeight(m_triangular->graph.end, m_rightShoulder->graph.start),
									1.0f };
	static int values_offset = 0;
	// draw gui
	ImGui::Text(m_name.c_str());
	ImGui::PushItemWidth(ImGui::GetContentRegionAvailWidth());
	ImGui::PlotLines("", values.data(), values.size(), values_offset, "", 0.0f, 1.0f, ImVec2(0, 80));
	ImGui::PopItemWidth();
	ImGui::InputFloat("LS start", &m_leftShoulder->graph.start, 1.0f, 1.0f, 2);
	ImGui::InputFloat("LS end", &m_leftShoulder->graph.end, 1.0f, 1.0f, 2);
	ImGui::InputFloat("TRI start", &m_triangular->graph.start, 1.0f, 1.0f, 2);
	ImGui::InputFloat("TRI peak", &m_triangular->graph.peak, 1.0f, 1.0f, 2);
	ImGui::InputFloat("TRI end", &m_triangular->graph.end, 1.0f, 1.0f, 2);
	ImGui::InputFloat("RS start", &m_rightShoulder->graph.start, 1.0f, 1.0f, 2);
	ImGui::InputFloat("RS end", &m_rightShoulder->graph.end, 1.0f, 1.0f, 2);
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

float LeftShoulderTriangularRightShoulder::lineHeight(float rightValue, float leftValue)
{
	float triRoot = (float)sqrt(3);
	float valley = ((rightValue - leftValue) * 0.5f) * triRoot;
	return valley / 100;
}
