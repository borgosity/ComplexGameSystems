#include "Behaviour.h"
#include "imgui_glfw3.h"
#include "FuzzyMemberSets.h"
#include "Agents.h"

Behaviour::Behaviour()
{
}

Behaviour::~Behaviour()
{
}
/// Draw Behaviour Membership Sets
/// - calls membership set to draw itself
void Behaviour::drawGUI(Agent & a_agent)
{
	std::string windowName = a_agent.name() + "'s " + traits.name + " Membership Sets";
	ImGui::Begin(windowName.c_str());
	ImGui::Columns(m_memberSets.size(), "mixed", false);
	// loops through member sets and draw data
	for (auto memberSet : m_memberSets)
	{
		memberSet->drawGUI();
		ImGui::NextColumn();
	}
	ImGui::End();
}

/// AND function retrun the lower value
float Behaviour::AND(float a_left, float a_right)
{
	if (a_left < a_right)
	{
		return a_left;
	}
	else
	{
		return a_right;
	}
}
/// OR function returns the larger value
float Behaviour::OR(float a_left, float a_right)
{
	if (a_left > a_right)
	{
		return a_left;
	}
	else
	{
		return a_right;
	}
}
/// NOT function returns the opposite value
float Behaviour::NOT(float a_value)
{
	return 1 - a_value;
}

void Behaviour::saveHistory(float a_currVal)
{
	if (a_currVal > 0) {
		if (traits.history.size() < traits.maxHistory) {
			traits.history.push_back(a_currVal);
		}
		else {
			traits.history.pop_front();
			traits.history.push_back(a_currVal);
		}
	}
}
/// gernate a values for a left/tri/right membership set
/// - left shoulder steep
/// - triangular broad
/// - right shoulder steep
std::vector<float> Behaviour::genBroadLTRSet(float a_min, float a_max)
{
	// left variables
	float leftShoulderMin = a_min;
	float leftShoulderMax = (a_max - a_min) / 3;
	// triangular variables
	float triangularMin = (leftShoulderMax - leftShoulderMin) / 3;
	float triangularPeak = a_max * 0.5f;
	float triangularMax = triangularPeak + triangularMin;
	// right variables
	float rightShoulderMin = (triangularMax - triangularMin);
	float rightShoulderMax = a_max;
	
	return std::vector<float>{
			leftShoulderMin,	
			leftShoulderMax,	
			triangularMin,		
			triangularPeak,		
			triangularMax,		
			rightShoulderMin,	
			rightShoulderMax	
	};
}
/// gernate a values for a left/tri/right membership set
/// - left shoulder broad
/// - triangular steep
/// - right shoulder broad
std::vector<float> Behaviour::genSteepLTRSet(float a_min, float a_max)
{
	// left variables
	float leftShoulderMin = a_min;
	float leftShoulderMax = a_max * 0.5f;
	// triangular variables
	float triangularMin = a_max * 0.25f;
	float triangularPeak = leftShoulderMax;
	float triangularMax = triangularPeak + triangularMin;
	// right variables
	float rightShoulderMin = triangularPeak;
	float rightShoulderMax = a_max;

	return std::vector<float>{
		leftShoulderMin,
			leftShoulderMax,
			triangularMin,
			triangularPeak,
			triangularMax,
			rightShoulderMin,
			rightShoulderMax
	};
}
