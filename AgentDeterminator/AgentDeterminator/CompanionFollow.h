#pragma once
#include <vector>
#include "Behaviour.h"
#include "FuzzyMemberSets.h"


class CompanionFollow :
	public Behaviour
{
public:
	CompanionFollow();
	CompanionFollow(float a_distanceMin, float distanceMax, float a_healthMin, float a_healthMax, float a_followMin, float a_followMax);
	CompanionFollow(float a_distanceCloseMin, float distanceCloseMax, float a_distanceMiddleMin, float a_distanceMiddleMid, float distanceMiddleMax, float a_distanceFarMin, float distanceFarMax,
			float a_healthLowMin, float healthLowMax, float a_healthOkayMin, float a_healthOkayMid, float healthOkayMax, float a_healthGoodMin, float healthGoodMax,
			float a_followLowMin, float followLowMax, float a_followMediumMin, float a_followMediumMid, float followMediumMax, float a_followHighMin, float followHighMax);
	virtual  ~CompanionFollow();

	void update(Agent & a_agent);

	std::vector<float> desireSettings() { return followable(); };

	// get fuzzy settings
	std::vector<float> distance() { return m_distanceSettings; };
	std::vector<float> health() { return m_healthSettings; };
	std::vector<float> followable() { return m_followSettings; };
	// set fuzzy settings
	std::vector<float> distance(std::vector<float> a_settings);
	std::vector<float> health(std::vector<float> a_settings);
	std::vector<float> followable(std::vector<float> a_settings);
	// destroy agent
	void destroy();

private:
	// distance
	LeftShoulderTriangularRightShoulder * m_distanceMS = nullptr;
	// health
	LeftShoulderTriangularRightShoulder * m_healthMS = nullptr;
	// evadeable
	LeftShoulderTriangularRightShoulder * m_followMS = nullptr;

	// fuzzy settings
	std::vector<float> m_distanceSettings;
	std::vector<float> m_healthSettings;
	std::vector<float> m_followSettings;
	// fill vectors function
	void initVectors();
};

