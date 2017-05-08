#pragma once
#include <vector>
#include "Behaviour.h"
#include "FuzzyMemberSets.h"

class EnemyFlee :
	public Behaviour
{
public:
	EnemyFlee();
	virtual ~EnemyFlee();
	EnemyFlee(float a_distanceMin, float distanceMax, float a_healthMin, float a_healthMax, float a_fleeMin, float a_fleeMax);
	EnemyFlee(float a_distanceCloseMin, float distanceCloseMax, float a_distanceMiddleMin, float a_distanceMiddleMid, float distanceMiddleMax, float a_distanceFarMin, float distanceFarMax,
		float a_healthLowMin, float healthLowMax, float a_healthOkayMin, float a_healthOkayMid, float healthOkayMax, float a_healthGoodMin, float healthGoodMax,
		float a_fleeLowMin, float fleeLowMax, float a_fleeMediumMin, float a_fleeMediumMid, float fleeMediumMax, float a_fleeHighMin, float fleeHighMax);

	void update(Agent & a_agent);

	std::vector<float> desireSettings() { return fleeable(); };

	// get fuzzy settings
	std::vector<float> distance() { return m_distanceSettings; };
	std::vector<float> health() { return m_healthSettings; };
	std::vector<float> fleeable() { return m_fleeSettings; };
	// set fuzzy settings
	std::vector<float> distance(std::vector<float> a_settings);
	std::vector<float> health(std::vector<float> a_settings);
	std::vector<float> fleeable(std::vector<float> a_settings);
	// destroy agent
	void destroy();

private:
	// distance
	LeftShoulderTriangularRightShoulder * m_distanceMS = nullptr;
	// health
	LeftShoulderTriangularRightShoulder * m_healthMS = nullptr;
	// evadeable
	LeftShoulderTriangularRightShoulder * m_fleeMS = nullptr;
	// fuzzy settings
	std::vector<float> m_distanceSettings;
	std::vector<float> m_healthSettings;
	std::vector<float> m_fleeSettings;
	// fill vectors function
	void initVectors();
};

