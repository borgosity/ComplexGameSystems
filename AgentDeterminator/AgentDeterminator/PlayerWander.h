#pragma once
#include <vector>
#include "Behaviour.h"
#include "FuzzyMemberSets.h"


class PlayerWander :
	public Behaviour
{
public:
	PlayerWander();
	PlayerWander(float a_distanceMin, float distanceMax, float a_healthMin, float a_healthMax, float a_wanderMin, float a_wanderMax);
	PlayerWander(float a_distanceCloseMin, float distanceCloseMax, float a_distanceMiddleMin, float a_distanceMiddleMid, float distanceMiddleMax, float a_distanceFarMin, float distanceFarMax,
		float a_healthLowMin, float healthLowMax, float a_healthOkayMin, float a_healthOkayMid, float healthOkayMax, float a_healthGoodMin, float healthGoodMax,
		float a_wanderLowMin, float wanderLowMax, float a_wanderMediumMin, float a_wanderMediumMid, float wanderMediumMax, float a_wanderHighMin, float wanderHighMax);
	virtual  ~PlayerWander();

	void update(Agent & a_agent);

	std::vector<float> desireSettings() { return wanderable(); };

	// get fuzzy settings
	std::vector<float> distance() { return m_distanceSettings; };
	std::vector<float> health() { return m_healthSettings; };
	std::vector<float> wanderable() { return m_wanderSettings; };
	// set fuzzy settings
	std::vector<float> distance(std::vector<float> a_settings);
	std::vector<float> health(std::vector<float> a_settings);
	std::vector<float> wanderable(std::vector<float> a_settings);
	// destroy agent
	void destroy();

private:
	// distance
	LeftShoulderTriangularRightShoulder * m_distanceMS = nullptr;
	// health
	LeftShoulderTriangularRightShoulder * m_healthMS = nullptr;
	// wanderable
	LeftShoulderTriangularRightShoulder * m_wanderMS = nullptr;
	// fuzzy settings
	std::vector<float> m_distanceSettings;
	std::vector<float> m_healthSettings;
	std::vector<float> m_wanderSettings;
	// fill vectors function
	void initVectors();
};

