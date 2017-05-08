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
