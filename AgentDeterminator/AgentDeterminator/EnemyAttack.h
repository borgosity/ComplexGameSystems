#pragma once
#include <vector>
#include "Behaviour.h"
#include "FuzzyMemberTypes.h"


class EnemyAttack :
	public Behaviour
{
public:
	EnemyAttack();
	EnemyAttack(float a_distanceMin, float distanceMax, float a_healthMin, float a_healthMax, float a_attackMin, float a_attackMax);
	EnemyAttack(float a_distanceCloseMin, float distanceCloseMax, float a_distanceMiddleMin, float a_distanceMiddleMid, float distanceMiddleMax, float a_distanceFarMin, float distanceFarMax,
		float a_healthLowMin, float healthLowMax, float a_healthOkayMin, float a_healthOkayMid, float healthOkayMax, float a_healthGoodMin, float healthGoodMax,
		float a_attackLowMin, float attackLowMax, float a_attackMediumMin, float a_attackMediumMid, float attackMediumMax, float a_attackHighMin, float attackHighMax);
	virtual  ~EnemyAttack();

	void update(Agent & a_agent);

	// get fuzzy settings
	std::vector<float> distance() { return m_distanceSettings; };
	std::vector<float> health() { return m_healthSettings; };
	std::vector<float> attackable() { return m_attackSettings; };
	// set fuzzy settings
	std::vector<float> distance(std::vector<float> a_settings);
	std::vector<float> health(std::vector<float> a_settings);
	std::vector<float> attackable(std::vector<float> a_settings);
	// destroy agent
	void destroy();

private:
	// distance
	FMF_LeftShoulder	* m_distanceClose = nullptr;
	FMF_Triangular		* m_distanceMiddle = nullptr;
	FMF_RightShoulder	* m_distanceFar = nullptr;
	// health
	FMF_LeftShoulder	* m_healthLow = nullptr;
	FMF_Triangular		* m_healthOkay = nullptr;
	FMF_RightShoulder	* m_healthGood = nullptr;
	// attackable
	FMF_LeftShoulder	* m_attackLow = nullptr;
	FMF_Triangular		* m_attackMedium = nullptr;
	FMF_RightShoulder	* m_attackHigh = nullptr;
	// fuzzy settings
	std::vector<float> m_distanceSettings;
	std::vector<float> m_healthSettings;
	std::vector<float> m_attackSettings;
	// fill vectors function
	void initVectors();
};

